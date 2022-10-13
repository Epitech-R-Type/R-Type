/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server() : _lobbyRunning(true) {}

int Server::setup() {
    this->launchGame();

    return 0;
}

int Server::mainLoop() {
    while (this->_lobbyRunning) {
        // LOBBY LOGIC
        // Note: the following is purely brainstorming, please give feedback

        // Check for new connections
        //   If new connection, add to lobby
        //   Lobby class would just be a basic class or struct storing info of connected clients
        // The check could also be implemented in the protocol implementation with the use of a
        // shared pointer to the Lobby class instance
    }

    return 0;
}

int Server::launchGame() {
    // this->_game = Game();
    this->_game.mainLoop();

    return 0;
}
