/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/ECSManager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Utilities/ray.hpp"
#include "GUI/Menus/Connection.hpp"

Client::Client() {
    this->_protocol = new ClientLobbyProtocol();

    this->_lobbyRunning = true;
    this->_connected = false;

    // Gangster Workaround to insure same comptype order client
    Utilities::createCompPoolIndexes();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    while (!IsWindowReady()) {
        //
    }
    SetTargetFPS(40);
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

bool Client::connect(std::string serverIP, int port) {
    this->_protocol->connect(serverIP, port);

    return this->_protocol->isConnected();
}

void Client::handleUserCommands() {
    std::optional<std::string> potMsg;
    while ((potMsg = this->_userCommands->pop())) {
        std::string msg = potMsg.value();

        if (msg == "Start")
            this->_protocol->sendStart();
    }
}

enum Stages {
    CONNECTION,
    ROOMSELECTION,
    ROOM,
    GAME,
};

int Client::mainLoop() {
    Connection connection(this);

    int stage = 0;
    bool sendStart = false;

    while (!connection.getDone()) {
        this->_protocol->handleIncMessages();
        this->handleUserCommands();

        connection.apply();
        connection.draw();
    }

    while (!(this->_connected = this->_protocol->isConnected())) {
    }

    this->_protocol->startGame();

    while (true) {
        const bool startGame = this->_protocol->shouldGameStart();

        if (startGame) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            this->launchGame();
        }
    }
    return 0;
}