/*
** EPITECH PROJECT, 2022
** Server.hpp
** File description:
** .
*/

#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Networking/UdpCommunication.hpp"
#include "Game.hpp"

// Root class for R-Type Server
// This class will handle the tcp communication required for game lobby
// forming along with launching games.
class Server {
    public:
    // Constructor: Instantiates TcpCommunication class
    Server();

    // Setup function
    // All setup goes in here, such as generation of all component ids
    // We could also send to the client information on the ECS such as MAX_COMPONENTS & MAX_ENTITIES
    int setup();

    // Main Loop
    int mainLoop();

    // Launches game
    // Note: Execution will hang until game is over,
    // in the future this can be done in a separate thread
    int launchGame();

    private:
    bool _lobbyRunning;
    Game _game;
};
