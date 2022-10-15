/*
** EPITECH PROJECT, 2022
** ClientGame.hpp
** File description:
** .
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Manager.hpp"
#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/UdpCommunication.hpp"
#include "../systems/Systems.hpp"
#include <memory>
#include <string>
#include <thread>

// This class implements the inpur, networking and graphical logic needed
// for the running of a game.
class ClientGame {
public:
    // Note: Construtor/Destructor shall be added as needed
    ClientGame(ECSManager* ECS, SpriteSystem* spriteSystem);
    ~ClientGame();

    void init();
    // Main loop
    void mainLoop();

private:
    ECSManager* _entManager;

    EntityID _player;

    SpriteSystem* _spriteSystem;
    VelocitySystem* _velocitySystem;
    PlayerMovementSystem* _playerMovementSystem;
    HealthSystem* _healthSystem;
    ArmamentSystem* _armamentSystem;
    HitboxSystem* _hitboxSystem;
    JanitorSystem* _janitorSystem;

    // Messaging queues for protocol
    // These should eventually be moved to the protocol class
    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;

    bool _isRunning;
    std::thread* _udpComThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
