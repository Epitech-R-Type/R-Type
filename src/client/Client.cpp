/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/Manager.hpp"
#include "raylib.h"

Client::Client() {

    this->_ECS = new ECSManager();
    this->_spriteSystem = new SpriteSystem(this->_ECS);
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
    this->_game = new ClientGame(this->_ECS, this->_spriteSystem);
    this->_game->init();
    this->_game->mainLoop();
    delete this->_ECS;
    delete this->_game;

    this->_ECS = new ECSManager();
    this->_spriteSystem = new SpriteSystem(this->_ECS);

    return 0;
}

void Client::connect(std::string serverIP, int port) {
    this->_protocol->connect(serverIP, port);
}

int Client::mainLoop() {

    while (this->_lobbyRunning)
        this->_protocol->handleMessages();

    return 0;
}