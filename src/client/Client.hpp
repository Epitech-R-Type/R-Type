/*
** EPITECH PROJECT, 2022
** Client.hpp
** File description:
** .
*/

#pragma once

#include "../shared/ECS/Manager.hpp"
#include "ClientGame/ClientGame.hpp"

// This class implementls the main loop for lobby handling of the client
// The lobby main logic will most likely depend on a local ecs
// As soon as a game start, this class will instantiate the ClientGame class
// responsible of everything pertaining to a given day
class Client {
    public:
        // Note: Construtor/Destructor shall be added as needed

        // Intialization of game lobby
        int setup();

        // Menu main loop
        // Return 0 if success or 1 if failure
        int mainLoop();

        // Launches new game
        // Return is 0 if success or 1 if failure
        int launchGame();

    private:
        bool _lobbyRunning;
        ClientGame _game;
};
