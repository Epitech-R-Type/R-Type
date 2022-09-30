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

#include "../../shared/MessageQueue.hpp"
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
        std::shared_ptr<MessageQueue<std::string>> _outgoingMsg;
        std::shared_ptr<MessageQueue<std::string>> _incomingMsg;

        std::thread _comThread;
};
