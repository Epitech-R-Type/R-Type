/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
{
    this->_incomingMsg = std::make_shared<MessageQueue<Message>>();
    this->_outgoingMsg = std::make_shared<MessageQueue<Message>>();
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
    std::optional<Message> msg;
    
    while (true) {
        while ((msg = this->_incomingMsg->pop())) {
            std::cout << "[Main Thread] Received msg : " << msg->msg << std::endl;
            std::cout << "[Main Thread] From : " << msg->client_id << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
