/*
** EPITECH PROJECT, 2022
** Game.cpp
** File description:
** .
*/

#include "Game.hpp"

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
    srand(time(0));
    this->_protocol.waitForClients();

    std::vector<Connection> connections = this->_protocol.getConnectedClients();

    for (int i = 0; i < connections.size(); i++)
        Factory::Ally::makePlayer(this->_entManager, i);

    // starts the music ingame
    this->_protocol.sendChangeMusic(NORMAL);
    LOG("Initializing game");
    this->loadLevel(1);
}

void Game::sendModified() {
    std::optional<EntityID> entityID;

    while ((entityID = this->_entManager->getModified())) {
        if (this->_entManager->isValidEntity(*entityID))
            this->_protocol.sendEntity(entityID.value());
        else
            this->_protocol.sendDelEntity(entityID.value());
    }
};

void Game::loadLevel(int nb)
{
    Level newLevel;
    const cmrc::file lvlFile = this->_fs.open(std::string("src/server/levels/level").append(std::to_string(nb)));
    std::string lvlStr = std::string(lvlFile.begin(), lvlFile.end());
    // if (lvlStr.size() != 0)
    //     LOG("is open");
    std::string line;
    Wave tmp;
    tmp.endless = false;
    std::vector<std::string> lvlArr = Utilities::splitStr(std::string(lvlStr), std::string("\n"));
    int i = 0;
    line = lvlArr[i++];
    Factory::Misc::makeBackground(this->_entManager, (Animation::AnimationID)std::atoi(line.c_str()));
    line = lvlArr[i++];
    newLevel.bossCountdown = std::atoi(line.c_str());

    while (i < lvlArr.size()) {
        line = lvlArr[i++];
        tmp.minSpawned = std::atoi(line.c_str());
        line = lvlArr[i++];
        tmp.maxSpawned = std::atoi(line.c_str());
        line = lvlArr[i++];
        tmp.spawnInterval = std::atof(line.c_str());
        srand(time(NULL));
        tmp.spawned = rand() % tmp.maxSpawned + tmp.minSpawned;
        tmp.enemy = {10, 10, Animation::AnimationID::Orb, 8.0, Armament::Type::Laser};
        newLevel.levelWaves.push_back(tmp);
        // LOG("new added");
    }
    newLevel.waveNb = 0;
    this->_currentLevel = newLevel;
    this->_level = nb;
    this->_bossTimer = getNow();
    this->_enemyTimer = getNow();
}

void Game::refreshLevel()
{
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_enemyTimer;

    // LOG("Still Running");
    if (elapsed_seconds.count() >= this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawnInterval && this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawned > 0) {
        this->_enemyTimer = getNow();
        this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawned--;
        Factory::Enemy::makeEnemy(this->_entManager, this->_currentLevel.levelWaves[this->_currentLevel.waveNb].enemy);
    }
    elapsed_seconds = now - this->_bossTimer;
    if (elapsed_seconds.count() >= this->_currentLevel.bossCountdown && this->_bossSpawned == false) {
        this->_bossTimer = getNow();
        this->_bossSpawned = true;
        Factory::Enemy::makeEndboss(this->_entManager);
        this->_protocol.sendChangeMusic(BOSS);
    }
    for (auto beg = this->_entManager->begin<Team::Component>(); beg != this->_entManager->end<Team::Component>(); ++beg) {
        Team::Component *ent = this->_entManager->getComponent<Team::Component>(*beg);
        if (*ent == Team::Enemy)
            return;
    }
    if (this->_bossSpawned)
        this->loadLevel(this->_level++);
    // check all entite with team component and see if someone is an enemy; if all enemys are dead increase waveNB or call loadLevel
}

int Game::mainLoop() {
    LOG("Starting Game");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    this->_bossTimer = getNow();
    this->_enemyTimer = getNow();

    while (this->_isRunning) {
        this->_protocol.handleCommands();
        this->_velocitySystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();

        // Always last
        this->_janitorSystem->apply();

        this->refreshLevel();

        this->sendModified();
    }

    return 0;
}
