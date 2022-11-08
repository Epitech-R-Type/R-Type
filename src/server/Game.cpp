/*
** EPITECH PROJECT, 2022
** Game.cpp
** File description:
** .
*/

#include "Game.hpp"
#include "../WindowsGuard.hpp"

#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/Serialization.hpp"
#include "Systems/Factory.hpp"

Game::Game(std::vector<Connection> connections, int port, UUIDM serverUUID)
    : _isRunning(true),
      _entManager(std::make_shared<ECSManager>()),
      _incomingMQ(std::make_shared<MessageQueue<Message<std::string>>>()),
      _outgoingMQ(std::make_shared<MessageQueue<Message<std::string>>>()),
      _protocol(_incomingMQ, _outgoingMQ, connections, _entManager, serverUUID) {
    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, port);

    // System initialization
    this->_velocitySystem = std::make_unique<VelocitySystem>(this->_entManager);
    this->_armamentSystem = std::make_unique<ArmamentSystem>(this->_entManager);
    this->_hitboxSystem = std::make_unique<HitboxSystem>(this->_entManager);
    this->_janitorSystem = std::make_unique<JanitorSystem>(this->_entManager);
}

Game::~Game() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

void Game::init() {
    LOG("Initializing game");
    srand(time(0));
    this->_protocol.waitForClients();

    Factory::Misc::makeBackground(this->_entManager);

    std::vector<Connection> connections = this->_protocol.getConnectedClients();

    for (auto conn : connections)
        Factory::Ally::makePlayer(this->_entManager, conn.player);

    // starts the music ingame
    this->_protocol.sendChangeMusic(1);
}

void Game::sendModified() {
    std::optional<EntityID> entityID;

    while ((entityID = this->_entManager->getModified())) {
        if (this->_entManager->isValidEntity(*entityID))
            this->_protocol.sendEntity(entityID.value());
        else
            this->_protocol.sendDelEntity(entityID.value());
    }
}

int Game::getPlayersAlive() {
    int count = 0;

    for (auto beg = this->_entManager->begin<Player::Component>(); beg != this->_entManager->end<Player::Component>(); ++beg)
        count++;

    return count;
};

int Game::mainLoop() {
    LOG("Starting Game");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Reset ALL client timeout timers
    this->_protocol.resetAllTimeouts();

    // ─── Timers ──────────────────────────────────────────────────────────────────────────────

    std::chrono::time_point<std::chrono::system_clock> timer = getNow();

    // Note : Boss will have to be scheduled later
    std::chrono::time_point<std::chrono::system_clock> bosstimer = getNow();
    bool bossSpawned = false;

    while (this->_isRunning && this->getPlayersAlive()) {
        // ─── Protocol Stuff ──────────────────────────────────────────────────────────────

        // Handle timed out clients
        this->_protocol.handleCommands();
        this->_protocol.handleDisconnectedClients();

        // ─── System Application ──────────────────────────────────────────────────────────

        this->_velocitySystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();
        this->_janitorSystem->apply(); // ALWAYS LAST

        // ─── Ennemy And Boss Spawning ────────────────────────────────────────────────────

        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - timer;

        if (elapsed_seconds.count() > 2) {
            Factory::Enemy::makeEnemy(this->_entManager);
            timer = getNow();
        }

        std::chrono::duration<double> elapsed_boss_seconds = now - bosstimer;

        if (elapsed_boss_seconds.count() > 20 && !bossSpawned) {
            Factory::Enemy::makeEndboss(this->_entManager);
            bossSpawned = true;
            // Changes the music to gamer-music because the boss spawned
            this->_protocol.sendChangeMusic(0);
        }

        this->sendModified();
    }

    // Signal end of game to all clients
    this->_protocol.sendGameEnd();

    LOG("GAME HAS ENDED");

    return 0;
}
