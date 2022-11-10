/*
** EPITECH PROJECT, 2022
** Client.hpp
** File description:
** .
*/

#pragma once

#include "../shared/ECS/ECS.hpp"
#include "ClientGame/ClientGame.hpp"
#include "Protocols/ClientLobbyProtocol.hpp"
#include "Protocols/TcpClient.hpp"
#include "Systems/MusicSystem.hpp"
#include "Systems/SpriteSystem.hpp"
#include <thread>

// This class implementls the main loop for lobby handling of the client
// The lobby main logic will most likely depend on a local ecs
// As soon as a game start, this class will instantiate the ClientGame class
// responsible of everything pertaining to a given day
class Client {
public:
    // Note: Construtor/Destructor shall be added as needed
    Client();
    // Intialization of game lobby

    // Menu main loop
    // Return 0 if success or 1 if failure
    int mainLoop();

    // Launches new game
    // Return is 0 if success or 1 if failure
    int launchGame();

    bool connect(std::string serverIP, int port = TCP_PORT);

    void handleUserCommands();

private:
    bool _lobbyRunning;
    bool _connected;

    std::shared_ptr<MessageQueue<std::string>> _userCommands = std::make_shared<MessageQueue<std::string>>();
    std::thread* _userInputThread;

    ClientGame* _game;
    ClientLobbyProtocol* _protocol;
};
