/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "ClientGame.hpp"

ClientGame::ClientGame() {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);
}

ClientGame::~ClientGame() {
    // Delete com thread
    delete this->_udpComThread;
}

void ClientGame::mainLoop() {
    std::cout << "Entering main loop()" << std::endl;

    while (this->_isRunning) {
        // Placeholder
    }

    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();
}
