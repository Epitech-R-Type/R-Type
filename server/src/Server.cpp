/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::setup()
{
    this->_comThread = std::thread(communication_main, this->_incomingMsg, this->_outgoingMsg);
}

void Server::main_loop()
{
    while (true) {
        std::cout << "Main thread going to sleep for 5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
