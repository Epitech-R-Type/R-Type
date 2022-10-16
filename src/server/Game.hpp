/*
** EPITECH PROJECT, 2022
** Game.hpp
** File description:
** .
*/

#pragma once

#include <memory>
#include <thread>

#include "../shared/ECS/Manager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Networking/UdpCommunication.hpp"
#include "Protocols/TcpServer.hpp"
#include "Systems/Systems.hpp"

// This class embodies everything having to do with a single game
// It will include:
//          - The game logic
//          - Udp communication handling
//          - Ecs handling
class Game {
public:
    // All the game setup is done in here
    Game();

    ~Game();

    void init();
    // Main Loop
    int mainLoop();

private:
    ECSManager* _entManager;
    VelocitySystem* _velocitySystem;
    ArmamentSystem* _armamentSystem;
    HitboxSystem* _hitboxSystem;
    JanitorSystem* _janitorSystem;

    // Messaging queues for protocol
    // These should eventually be moved to the protocol class
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    bool _isRunning;
    std::thread* _udpComThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
