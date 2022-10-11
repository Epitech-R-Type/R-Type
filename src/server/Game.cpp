/*
** EPITECH PROJECT, 2022
** Game.cpp
** File description:
** .
*/

#include "Game.hpp"

Game::Game() : _isRunning(true) {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // Init com thread
    spawnUDPThread(this->_incomingMQ, this->_outgoingMQ);
}

int Game::mainLoop() {
    std::cout << "Entering main loop()" << std::endl;

    while (this->_isRunning) {
        std::optional<Message<std::string>> msg;

        if ((msg = this->_incomingMQ->pop())) {
            std::cout << "Received :" << *msg << std::endl;

            // Send back same message
            this->_outgoingMQ->push(*msg);
        }
    }
}
