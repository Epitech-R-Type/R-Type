/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
    : _lobbyRunning(true),
      _serverUUID() {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // For some reason initializer list initialization wasn't working
    this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ, this->_serverUUID);

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
    int port;

    while (this->_lobbyRunning) {
        // DO STUFF

        // Handle messages and launch game if game should start
        if ((port = this->_protocol->handleCommands())) {
            this->launchGame(port);
        }
    }

    return 0;
}

int Server::launchGame(int port) {
    this->_game = new Game(this->_protocol->getConnections(), port, this->_serverUUID);

    this->_game->init();
    this->_game->mainLoop();

    delete this->_game;

    return 0;
}
