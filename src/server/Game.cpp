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

// Main function to be given to thread
void game_main(std::vector<Connection> connections, int port, Utilities::UUID serverUUID, std::shared_ptr<std::atomic<bool>> gameStopFlag) {
    Game game(connections, port, serverUUID, gameStopFlag);

    // Run game
    game.init();
    game.mainLoop();
}

Game::Game(std::vector<Connection> connections, int port, Utilities::UUID serverUUID, std::shared_ptr<std::atomic<bool>> gameStopFlag)
    : _isRunning(true),
      _entManager(std::make_shared<ECSManager>()),
      _incomingMQ(std::make_shared<MessageQueue<Message<ByteBuf>>>()),
      _outgoingMQ(std::make_shared<MessageQueue<Message<ByteBuf>>>()),
      _protocol(_incomingMQ, _outgoingMQ, connections, _entManager, serverUUID) {
    this->_gameStopFlag = gameStopFlag;

    // Init com thread
    this->_udpStopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_udpStopFlag, port);

    // System initialization
    this->_velocitySystem = std::make_unique<VelocitySystem>(this->_entManager);
    this->_armamentSystem = std::make_unique<ArmamentSystem>(this->_entManager);
    this->_hitboxSystem = std::make_unique<HitboxSystem>(this->_entManager);
    this->_janitorSystem = std::make_unique<JanitorSystem>(this->_entManager);
}

Game::~Game() {
    // Signal thread to stop and join thread
    this->_udpStopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;

    // Signal thread has stopped
    this->_gameStopFlag->store(true);
}

void Game::init() {
    srand(time(0));
    this->_protocol.waitForClients();

    std::vector<Connection> connections = this->_protocol.getConnectedClients();

    for (auto conn : connections)
        Factory::Ally::makePlayer(this->_entManager, conn.player);

    // starts the music ingame
    // this->_protocol.sendChangeMusic(NORMAL);
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
}

int Game::getPlayersAlive() {
    int count = 0;

    for (auto beg = this->_entManager->begin<Player::Component>(); beg != this->_entManager->end<Player::Component>(); ++beg)
        count++;

    return count;
};

bool Game::loadLevel(int nb) {
    if (this->_fs.is_file(std::string("src/server/levels/level").append(std::to_string(nb))) == false)
        return false;

    Level newLevel;
    const cmrc::file lvlFile = this->_fs.open(std::string("src/server/levels/level").append(std::to_string(nb)));
    std::string lvlStr = std::string(lvlFile.begin(), lvlFile.end());
    std::string line;
    Wave tmp;
    tmp.endless = false;
    std::vector<std::string> lvlArr = Utilities::splitStr(std::string(lvlStr), std::string("\n"));
    int i = 0;
    line = lvlArr[i++];
    Factory::Misc::makeBackground(this->_entManager, (Animation::AnimationID)std::atoi(line.c_str()));
    line = lvlArr[i++];
    this->_protocol.sendChangeMusic((SongID)std::atoi(line.c_str()));
    bool noBoss = false;

    while (i < lvlArr.size()) {
        line = lvlArr[i++];
        tmp.minSpawned = std::atoi(line.c_str());
        line = lvlArr[i++];
        tmp.maxSpawned = std::atoi(line.c_str());
        line = lvlArr[i++];
        tmp.spawnInterval = std::atof(line.c_str());
        tmp.spawned = rand() % tmp.maxSpawned + tmp.minSpawned;
        line = lvlArr[i++];
        std::vector<std::string> lineArr = Utilities::splitStr(line, " ");
        for (auto is : lineArr) {
            tmp.enemies.push_back(this->_enemys[std::stoi(is)]);
        }
        line = lvlArr[i++];
        if (std::atoi(line.c_str()) == -1) {
            tmp.boss = this->_bosses[0];
            noBoss = true;
        } else
            tmp.boss = this->_bosses[std::atoi(line.c_str())];
        newLevel.levelWaves.push_back(tmp);
    }

    newLevel.waveNb = 0;
    this->_currentLevel = newLevel;
    this->_level = nb;
    this->_enemyTimer = getNow();
    this->_bossSpawned = noBoss;
    return true;
}

void Game::refreshLevel() {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_enemyTimer;

    if (elapsed_seconds.count() >= this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawnInterval &&
        this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawned > 0) {
        this->_enemyTimer = getNow();
        this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawned--;
        int rd = rand() % this->_currentLevel.levelWaves[this->_currentLevel.waveNb].enemies.size();
        Factory::Enemy::makeEnemy(this->_entManager, this->_currentLevel.levelWaves[this->_currentLevel.waveNb].enemies[rd]);
    }
    bool allSpawned = false;

    if (this->_currentLevel.levelWaves[this->_currentLevel.waveNb].spawned == 0) { // next wave of enemies
        allSpawned = true;
        if (noEnemies()) {
            this->_currentLevel.waveNb += 1;
        }
    }

    if (this->_currentLevel.waveNb >= this->_currentLevel.levelWaves.size() && !this->_bossSpawned && allSpawned) { // spawning boss
        this->_currentLevel.waveNb -= 1;
        this->_bossSpawned = true;
        Factory::Enemy::makeEndboss(this->_entManager, this->_currentLevel.levelWaves[this->_currentLevel.levelWaves.size() - 1].boss);
        this->_protocol.sendChangeMusic(BOSS);
    }

    if (noEnemies() && this->_bossSpawned && allSpawned) { // loading next level
        if (this->loadLevel(++this->_level) == false) {
            this->_isRunning = false;
        }
    }
}

bool Game::noEnemies() {
    bool noEnemy = true;

    for (auto beg = this->_entManager->begin<Team::Component>(); beg != this->_entManager->end<Team::Component>(); ++beg) {
        Team::Component* ent = this->_entManager->getComponent<Team::Component>(*beg);
        if (ent->team == Team::Group::Enemy) {
            noEnemy = false;
            break;
        }
    }

    return noEnemy;
}

int Game::mainLoop() {
    LOG("Starting Game");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    this->_enemyTimer = getNow();

    // Reset ALL client timeout timers
    this->_protocol.resetAllTimeouts();

    // ─── Timers ──────────────────────────────────────────────────────────────────────────────

    bool bossSpawned = false;

    while (this->_isRunning && this->getPlayersAlive() && !*this->_gameStopFlag) {
        // ─── Protocol Stuff ──────────────────────────────────────────────────────────────

        // Handle timed out clients
        this->_protocol.handleCommands();
        this->_protocol.handleDisconnectedClients();

        // Handle death dispensing
        for (auto victim : this->_janitorSystem->getKilledPlayers())
            this->_protocol.sendDeath(victim);
        this->_janitorSystem->resetKilledPlayers();

        // ─── System Application ──────────────────────────────────────────────────────────

        this->_velocitySystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();
        this->_janitorSystem->apply(); // ALWAYS LAST

        // ─── Ennemy And Boss Spawning ────────────────────────────────────────────────────

        this->refreshLevel();

        this->sendModified();
    }

    // Signal end of game to all clients
    this->_protocol.sendGameEnd();

    LOG("GAME HAS ENDED");

    return 0;
}
