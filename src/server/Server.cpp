/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
    : _lobbyRunning(true) {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // For some reason initializer list initialization wasn't working
    this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ);

    // Init tcp com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);

    // Gangster Workaround to insure same comptype order client and server side
    Utilities::createCompPoolIndexes();
}

Server::~Server() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_comThread->join();

    // Delete com thread
    delete this->_comThread;
    // Delete protocol implementation
    delete this->_protocol;
}

int Server::setup() {
    return 0;
}

int Server::mainLoop() {
    while (this->_lobbyRunning) {
        // DO STUFF

        // Handle messages and launch game if game should start
        if (this->_protocol->handleCommands())
            this->launchGame();
    }

    return 0;
}

int Server::launchGame() {
    this->_game.mainLoop();

    return 0;
}
