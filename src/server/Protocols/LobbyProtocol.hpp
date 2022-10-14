/*
** EPITECH PROJECT, 2022
** LobbyProtocol.hpp
** File description:
** .
*/

#pragma once

#include <asio.hpp>
#include <memory>
#include <sstream>

#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "Connections.hpp"

// Generic protocol pieces
#define SP " "
#define END "\r\n"
#define EMPTY_ARGS "X"

// Server Commands
#define START_GAME "START"

class LobbyProtocol {
public:
    LobbyProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

    // Server Commands
    // Sends start game command to every connected client
    void startGame();

    // Utility function to check if users UUID is valid
    bool isAuthenticated(std::string uuid);

    // Handle commands
    // Returns true if game should start
    bool handleCommands();

private:
    // Connection manager
    ConnectionManager _connMan;

    // Messaging Queues
    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;
};
