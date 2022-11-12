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

struct GameInfo {
    int lobbyId;
    int port;
};

class LobbyProtocol {
public:
    LobbyProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                  Utilities::UUID serverUUID, std::shared_ptr<std::vector<GameInfo>> gamesToLaunch);

    // Server Commands
    // Sends start game command to every connected client
    void startGame(int port, int lobby);

    // ─── Handle Commands And Responde ────────────────────────────────────────────────────────

    // Handle commands
    void handleCommands();

    void handleWrongRequest(std::string msgBody, asio::ip::address addr, asio::ip::port_type port);
    void handleForbidden(asio::ip::address addr, asio::ip::port_type port);
    void handleConnect(asio::ip::address addr, asio::ip::port_type port);
    void handleJoinLobby(asio::ip::address addr, asio::ip::port_type port, Utilities::UUID clientUUID, int lobby);
    void handleStart(Utilities::UUID uuid, asio::ip::address addr, asio::ip::port_type port);
    void handleGetLobbies(asio::ip::address addr, asio::ip::port_type port);
    void handleLeave(Utilities::UUID uuid, asio::ip::address addr, asio::ip::port_type port);

    // ─── Utility Functions ───────────────────────────────────────────────────────────────────

    // Utility function in order to form response automatically
    // Handles putting uuid in and str forming
    void sendResponse(std::string code, std::string args, asio::ip::address addr, asio::ip::port_type port);

    // Utility function to check if users UUID is valid
    bool isAuthenticated(std::string uuid);

    // Retrieve all connected clients
    std::vector<Connection> getConnections();

    // ─── Getters And Setters ─────────────────────────────────────────────────────────────────

    ConnectionManager& getConnectionManager();

    // Updated by Server class for handling start game
    void addRunningLobby(int lobby);
    void removeRunningLobbies(int lobby);
    bool isLobbyRunning(int lobby);

private:
    // Connection manager
    ConnectionManager _connMan;

    // Messaging Queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    // Lobbies currently in game
    std::vector<int> _runningLobbies;

    // Games to launch reference
    std::shared_ptr<std::vector<GameInfo>> _gamesToLaunch;
};
