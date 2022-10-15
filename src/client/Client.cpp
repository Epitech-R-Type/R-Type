/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"

int Client::launchGame() {
    // Note: For performance reasons we could free the lobby ecs before launching the game
    this->_game.init();
    this->_game.mainLoop();

    return 0;
}

int Client::mainLoop() {
    while (this->_lobbyRunning && !WindowShouldClose()) {
        // Lobby logic
        // Use input from user to navigate menu
        // Manipulate local ecs
    }

    return 0;
}

int Client::setup() {
    return 0;
}
