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
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);
}

Game::~Game() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

int Game::mainLoop() {

    std::cout << "Entering main loop()" << std::endl;

    while (this->_isRunning) {
        std::optional<Message<std::string>> msg;

        if ((msg = this->_incomingMQ->pop())) {
            std::cout << "Received :" << *msg << std::endl;
            std::string contents;
            contents << *msg;

            if (contents == "quit\n") {
                std::cout << "In if" << std::endl;
                this->_isRunning = false;
            }

            // Send back same message
            this->_outgoingMQ->push(*msg);
        }
    }

    return 0;
}
