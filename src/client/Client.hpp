/*
** EPITECH PROJECT, 2022
** Client.hpp
** File description:
** .
*/

#pragma once

#include "ClientGame/ClientGame.hpp"
#include "Protocols/TcpClient.hpp"
#include <thread>

// This class implementls the main loop for lobby handling of the client
// The lobby main logic will most likely depend on a local ecs
// As soon as a game start, this class will instantiate the ClientGame class
// responsible of everything pertaining to a given day
class Client {
public:
    // Note: Construtor/Destructor shall be added as needed
    Client() {
        this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
        this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

        // For some reason initializer list initialization wasn't working
        // this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ);

        // Init tcp com thread
        this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
        this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);
    }
    // Intialization of game lobby

    // Menu main loop
    // Return 0 if success or 1 if failure
    int mainLoop();

    // Launches new game
    // Return is 0 if success or 1 if failure
    int launchGame();

private:
    bool _lobbyRunning;
    ClientGame _game;

    // LobbyProtocol* _protocol;

    // Tcp com thread stuff
    std::thread* _comThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;

    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;
};
