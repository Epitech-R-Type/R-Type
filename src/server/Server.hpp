/*
** EPITECH PROJECT, 2022
** Server.hpp
** File description:
** .
*/

#pragma once

#include <string>
#include <thread>
#include <memory>
#include <chrono>
#include <iostream>

#include "../shared/MessageQueue/MessageQueue.hpp"
#include "Communication.hpp"

// Root class for R-Type Server
class Server {
    public:
        Server();
        ~Server();

        void setup();
        void run();
        void main_loop();

    private:
        std::shared_ptr<MessageQueue<Message>> _outgoingMsg;
        std::shared_ptr<MessageQueue<Message>> _incomingMsg;

        std::thread _comThread;
};
