/*
** EPITECH PROJECT, 2022
** ClientGame.hpp
** File description:
** .
*/

#pragma once

#include <memory>
#include <thread>

#include "../../shared/ECS/Manager.hpp"
#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/UdpCommunication.hpp"

// This class implements the inpur, networking and graphical logic needed
// for the running of a game.
class ClientGame {
    public:
        // Note: Construtor/Destructor shall be added as needed
        ClientGame();
        ~ClientGame();

        // Main loop
        void mainLoop();

    private:
        Manager _entManager;

        // Messaging queues for protocol
        // These should eventually be moved to the protocol class
        std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
        std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;

        bool _isRunning;
        std::thread* _udpComThread;
        std::shared_ptr<std::atomic<bool>> _stopFlag;
};
