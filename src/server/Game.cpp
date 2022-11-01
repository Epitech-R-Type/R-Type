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

Game::Game(std::vector<Connection> connections, int port)
    : _isRunning(true),
      _entManager(std::make_shared<ECSManager>()),
      _incomingMQ(std::make_shared<MessageQueue<Message<std::string>>>()),
      _outgoingMQ(std::make_shared<MessageQueue<Message<std::string>>>()),
      _protocol(_incomingMQ, _outgoingMQ, connections, _entManager) {
    // Construct messaging queues

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, port);

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

    for (int i = 0; i < connections.size(); i++)
        Factory::Ally::makePlayer(this->_entManager, i);
}

void Game::sendModified() {
    std::optional<EntityID> entityID;

    while ((entityID = this->_entManager->getModified())) {
        if (this->_entManager->entityIsActive(getIndex(entityID.value())) && this->_entManager->isValidEntity(entityID.value()))
            this->_protocol.sendEntity(entityID.value());
        else
            this->_protocol.sendDelEntity(entityID.value());
    }
};

int Game::mainLoop() {
    LOG("Starting Game");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::chrono::time_point<std::chrono::system_clock> timer = getNow();
    std::chrono::time_point<std::chrono::system_clock> bosstimer = getNow();
    bool bossSpawned = false;
    //starts the music ingame
    this->_protocol.sendChangeMusic(1);

    while (this->_isRunning) {
        this->_protocol.handleCommands();
        this->_velocitySystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();

        // Always last
        this->_janitorSystem->apply();

        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - timer;

        // Convert to milliseconds
        if (elapsed_seconds.count() > 2) {
            Factory::Enemy::makeEnemy(this->_entManager);
            timer = getNow();
        }

        std::chrono::duration<double> elapsed_boss_seconds = now - bosstimer;

        if (elapsed_boss_seconds.count() > 20 && !bossSpawned) {
            Factory::Enemy::makeEndboss(this->_entManager);
            bossSpawned = true;
            //Changes the music to gamer-music because the boss spawned
            this->_protocol.sendChangeMusic(0);
        }

        this->sendModified();
    }

    return 0;
}
