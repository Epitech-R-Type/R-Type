/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/ECSManager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "raylib.h"

Client::Client() {
    this->_protocol = new ClientLobbyProtocol();

    this->_lobbyRunning = true;
    this->_connected = false;

    // Gangster Workaround to insure same comptype order client
    Utilities::createCompPoolIndexes();
}

int Client::launchGame() {
    if (!this->_connected)
        return 84;

    // Note: For performance reasons we could free the lobby ecs before launching the game
    this->_game = new ClientGame(this->_protocol->getUUID(), this->_protocol->getServerIp(), this->_protocol->getServerPort());
    this->_game->init();
    this->_game->mainLoop();
    delete this->_game;

    return 0;
}

int Client::connect(std::string serverIP, int port) {
    return this->_protocol->connect(serverIP, port);
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
        if (this->_protocol->handleIncMessages())
            return 0;

        this->handleUserCommands();

        this->_connected = this->_protocol->isConnected();

        if (this->_protocol->shouldGameStart()) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            this->launchGame();
        }
    }

    return 0;
}