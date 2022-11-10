/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server()
    : _serverUUID() {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // Games to launch vector init
    this->_gamesToLaunch = std::make_shared<std::vector<GameInfo>>();

    // For some reason initializer list initialization wasn't working
    this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ, this->_serverUUID, this->_gamesToLaunch);

    // Init tcp com thread
    this->_tcpStopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_tcpComThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_tcpStopFlag);

    // Gangster Workaround to insure same comptype order client and server side
    Utilities::createCompPoolIndexes();
}

Server::~Server() {
    // Signal tcp com thread to stop and join thread
    this->_tcpStopFlag->store(true);
    this->_tcpComThread->join();

    // Delete tcp com thread
    delete this->_tcpComThread;
    // Delete protocol implementation
    delete this->_protocol;
}

int Server::setup() {
    return 0;
}

int Server::mainLoop() {
    int port;

    while (true) {
        // Handle messages
        this->_protocol->handleCommands();

        // Launch new games
        this->handleNewGames();
    }

    return 0;
}

void Server::launchGame(int port) {
    this->_game = new Game(this->_protocol->getConnections(), port, this->_serverUUID);

    this->_game->init();
    this->_game->mainLoop();

    delete this->_game;
}

// ─── Game Lobbies Handling ───────────────────────────────────────────────────────────────────────

void Server::handleNewGames() {
    for (auto game : *this->_gamesToLaunch)
        launchGame(game.port);

    this->_gamesToLaunch->clear();
}
