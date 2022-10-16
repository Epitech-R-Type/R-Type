/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/Manager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "raylib.h"

Client::Client() {
    this->_protocol = new ClientLobbyProtocol();

    this->_lobbyRunning = true;
    this->_connected = false;
}

int Client::launchGame() {
    if (!this->_connected) {
        std::cout << "You are not connected to a server" << std::endl;
        return 84;
    }

    // Note: For performance reasons we could free the lobby ecs before launching the game
    this->_game = new ClientGame();
    this->_game->init();
    this->_game->mainLoop();
    delete this->_game;

    return 0;
}

void Client::connect(std::string serverIP, int port) {
    this->_protocol->connect(serverIP, port);
}

void Client::handleUserCommands() {
    std::optional<std::string> potMsg;
    while ((potMsg = this->_userCommands->pop())) {
        std::string msg = potMsg.value();

        if (msg == "Start")
            this->_protocol->sendStart();
    }
}

void userInput(std::shared_ptr<MessageQueue<std::string>> userCommands) {
    while (1) {
        std::string command;

        std::cin >> command;

        userCommands->push(command);
    }
}

int Client::mainLoop() {
    this->_userInputThread = new std::thread(userInput, this->_userCommands);

    while (this->_lobbyRunning) {
        this->_protocol->handleIncMessages();

        this->handleUserCommands();

        this->_connected = this->_protocol->isConnected();

        if (this->_protocol->shouldGameStart()) {
            this->launchGame();
        }
    }

    return 0;
}