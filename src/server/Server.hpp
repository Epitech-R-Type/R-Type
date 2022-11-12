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
#include "Game.hpp"
#include "Protocols/LobbyProtocol.hpp"
#include "Protocols/TcpServer.hpp"

// Root class for R-Type Server
// This class will handle the tcp communication required for game lobby
// forming along with launching games.
class Server {
public:
    // Constructor: Instantiates x class and launch com thread
    Server();
    ~Server();

    // Setup function
    // All setup goes in here, such as generation of all component ids
    // We could also send to the client information on the ECS such as MAX_COMPONENTS & MAX_ENTITIES
    int setup();

    // Main Loop
    int mainLoop();

    // Launches game in separate thread
    void launchGame(GameInfo info);

    // ─── Game Lobbies Handling ───────────────────────────────────────────────────────

    // Check if new games to launch and launch them
    void handleNewGames();

    // Check if game has ended and cleanup
    void handleFinishedGames();

    // Quit all game threads
    // Note: Exec hangs until all threads joined
    void stopGames();

private:
    Game* _game;
    LobbyProtocol* _protocol;
    Utilities::UUID _serverUUID;

    // Tcp com thread stuff
    std::thread* _tcpComThread;
    std::shared_ptr<std::atomic<bool>> _tcpStopFlag;

    // Tcp message queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    // ─── All Same Size ───────────────────────────────────────────────────────────────

    // Lobby handling
    std::vector<int> _runningLobbies;
    // Lobby thread handling
    std::shared_ptr<std::vector<GameInfo>> _gamesToLaunch;
    std::vector<std::unique_ptr<std::thread>> _gameThreads;
    std::vector<std::shared_ptr<std::atomic<bool>>> _gameStopFlags;
};
