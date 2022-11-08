/*
** EPITECH PROJECT, 2022
** Game.hpp
** File description:
** .
*/

#pragma once

#include <memory>
#include <thread>
#include <fstream>
#include <filesystem>

#include "../shared/ECS/ECSManager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Networking/UdpCommunication.hpp"
#include "Protocols/GameProtocol.hpp"
#include "Protocols/TcpServer.hpp"
#include "Systems/Systems.hpp"

// This class embodies everything having to do with a single game
// It will include:
//          - The game logic
//          - Udp communication handling
//          - Ecs handling

struct Wave {
    bool endless;
    int minSpawned;
    int maxSpawned;
    double spawnInterval;
    int spawned;
    Enemy enemy;
};

struct Level {
    int waveNb;
    int bossCountdown;
    int mapId;
    std::vector<Wave> levelWaves;
};

class Game {
public:
    // All the game setup is done in here
    Game(std::vector<Connection> connections, int port);

    ~Game();

    void init();
    // Main Loop
    int mainLoop();

    void sendModified();

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
    timePoint _bossTimer;
    int _level;
    Level _currentLevel;
    void loadLevel(int nb);
    void refreshLevel();

    // Multithreading
    bool _isRunning;
    std::thread* _udpComThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
