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
#include "../../shared/Utilities/UUID.hpp"
#include "../Protocols/ClientGameProtocol.hpp"
#include "../Systems/Systems.hpp"
#include <memory>
#include <string>
#include <thread>

// This class implements the inpur, networking and graphical logic needed
// for the running of a game.
class ClientGame {
public:
    // Note: Construtor/Destructor shall be added as needed
    ClientGame(UUIDM uuid, asio::ip::address addr, int port);
    ~ClientGame();

    void init();
    // Main loop
    void mainLoop();

private:
    EntityID _player = INVALID_INDEX;

    std::shared_ptr<ECSManager> _entManager;
    std::unique_ptr<SpriteSystem> _spriteSystem;
    std::unique_ptr<VelocitySystem> _velocitySystem;
    std::unique_ptr<PlayerMovementSystem> _playerMovementSystem;
    std::unique_ptr<HealthSystem> _healthSystem;

    // Messaging queues for protocol
    // These should eventually be moved to the protocol class
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    ClientGameProtocol _protocol;

    UUIDM _uuid;

    bool _isRunning;
    std::thread* _udpComThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
