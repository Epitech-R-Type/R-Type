/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
{
    this->_incomingMsg = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMsg = std::make_shared<MessageQueue<std::string>>();
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
    std::optional<std::string> msg;
    
    while (true) {
        std::cout << "[Main Thread] Checking messaging queue for new messages..." << std::endl;
        while ((msg = this->_incomingMsg->pop())) {
            std::cout << "[Main Thread] Received msg : " << *msg << std::endl;            
        }
        std::cout << "[Main Thread] DONE checking messaging queue..." << std::endl;
        
        std::cout << "[Main Thread] Main thread going to sleep for 5s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
