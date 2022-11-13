/*
** EPITECH PROJECT, 2022
** Game.hpp
** File description:
** .
*/

#pragma once

#include <cmrc/cmrc.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <thread>

#include "../shared/ECS/ECSManager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Networking/UdpCommunication.hpp"
#include "Protocols/GameProtocol.hpp"
#include "Protocols/TcpServer.hpp"
#include "Systems/Systems.hpp"

CMRC_DECLARE(server);

void game_main(std::vector<Connection> connections, int port, Utilities::UUID serverUUID, std::shared_ptr<std::atomic<bool>> gameStopFlag);

// This class embodies everything having to do with a single game
// It will include:
//          - The game logic
//          - Udp communication handling
//          - Ecs handling

struct EnemyStats {
    int health;
    int damage;
    Animation::AnimationID sprite;
    float speed;
    Armament::Type armament;
};

struct BossStats {
    int health;
    int damage;
    Animation::AnimationID sprite;
    Armament::Type armament;
};

struct Wave {
    bool endless;
    int minSpawned;
    int maxSpawned;
    double spawnInterval;
    int spawned;
    std::vector<EnemyStats> enemies;
    BossStats boss;
};

struct Level {
    int waveNb;
    int bossCountdown;
    std::vector<Wave> levelWaves;
};

class Game {
public:
    // All the game setup is done in here
    Game(std::vector<Connection> connections, int port, Utilities::UUID serverUUID, std::shared_ptr<std::atomic<bool>> gameStopFlag);

    ~Game();

    void init();
    // Main Loop
    int mainLoop();

    void sendModified();
    int getPlayersAlive();

private:
    // ECS and Systems
    std::shared_ptr<ECSManager> _entManager;
    std::unique_ptr<VelocitySystem> _velocitySystem;
    std::unique_ptr<ArmamentSystem> _armamentSystem;
    std::unique_ptr<HitboxSystem> _hitboxSystem;
    std::unique_ptr<JanitorSystem> _janitorSystem;

    // UDP Networking
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;
    GameProtocol _protocol;

    // Game Logic
    bool _bossSpawned = false;
    timePoint _enemyTimer;
    int _level;
    Level _currentLevel;
    std::vector<EnemyStats> _enemys = {
        {10, 5, Animation::AnimationID::Orb, 8.0, Armament::Type::Laser},
        {20, 25, Animation::AnimationID::Orb, 10.0, Armament::Type::Laser},
        {30, 15, Animation::AnimationID::Orb, 3.0, Armament::Type::Laser},
        {50, 50, Animation::AnimationID::Orb, 1.0, Armament::Type::Laser},
        {100, 50, Animation::AnimationID::LegRobot, 1.0, Armament::Type::Laser},
        {100, 50, Animation::AnimationID::Transformer, 1.0, Armament::Type::Ball},
    };
    std::vector<BossStats> _bosses = {
        {300, 20, Animation::AnimationID::Cluster, Armament::Type::Buckshot},
        {400, 50, Animation::AnimationID::Cluster, Armament::Type::Laser},
    };
    bool loadLevel(int nb);
    void refreshLevel();
    bool noEnemies();
    cmrc::embedded_filesystem _fs = cmrc::server::get_filesystem();

    // Multithreading
    bool _isRunning;
    std::thread* _udpComThread;
    std::shared_ptr<std::atomic<bool>> _udpStopFlag;
    std::shared_ptr<std::atomic<bool>> _gameStopFlag;
};
