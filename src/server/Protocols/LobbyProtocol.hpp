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
#include "../../shared/Networking/AsioConstants.hpp"
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
    LobbyProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                  Utilities::UUID serverUUID);

    // Server Commands
    // Sends start game command to every connected client
    void startGame(int port);

    // ─── Handle Commands And Responde ────────────────────────────────────────────────────────

    // Handle commands
    // Returns true if game should start
    int handleCommands();

    void handleWrongRequest(std::string msgBody, asio::ip::address addr, asio::ip::port_type port);
    void handleForbidden(asio::ip::address addr, asio::ip::port_type port);
    void handleConnect(asio::ip::address addr, asio::ip::port_type port);
    int handleStart();

    // ─── Utility Functions ───────────────────────────────────────────────────────────────────

    // Utility function in order to form response automatically
    // Handles putting uuid in and str forming
    void sendResponse(std::string code, std::string args, asio::ip::address addr, asio::ip::port_type port);

    // Utility function to check if users UUID is valid
    bool isAuthenticated(std::string uuid);

    std::vector<Connection> getConnections();

private:
    // Connection manager
    ConnectionManager _connMan;

    // Messaging Queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;
};
