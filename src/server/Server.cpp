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

    // Stop all games
    this->stopGames();

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

        // Handle finished games
        this->handleFinishedGames();
    }

    return 0;
}

void Server::launchGame(GameInfo info) {
    // Create stop flag
    std::shared_ptr<std::atomic<bool>> gameStopFlag = std::make_shared<std::atomic<bool>>(false);
    // Create new thread for new game
    std::unique_ptr<std::thread> newGameThread =
        std::make_unique<std::thread>(game_main, this->_protocol->getConnections(), info.port, this->_serverUUID, gameStopFlag);

    // Push back to vectors
    this->_gameThreads.push_back(std::move(newGameThread));
    this->_gameStopFlags.push_back(std::move(gameStopFlag));
}

// ─── Game Lobbies Handling ───────────────────────────────────────────────────────────────────────

void Server::handleNewGames() {
    for (auto game : *this->_gamesToLaunch)
        launchGame(game);

    this->_gamesToLaunch->clear();
}

void Server::handleFinishedGames() {
    // Delete games whose game stop flag has been set
    for (int i = 0; i < this->_gameThreads.size(); i++)
        if (*this->_gameStopFlags[i]) {
            // Join thread just in case
            this->_gameThreads[i]->join();

            // Cleanup
            this->_gameThreads.erase(this->_gameThreads.begin() + i);
            this->_gameStopFlags.erase(this->_gameStopFlags.begin() + i);
        }
}

void Server::stopGames() {
    // Exit main loops
    for (auto& stopFlag : this->_gameStopFlags)
        stopFlag->store(true);

    // Join threads
    for (auto& thread : this->_gameThreads)
        thread->join();
}
