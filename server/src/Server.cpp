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

}
