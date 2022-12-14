/*
** EPITECH PROJECT, 2022
** Connections.hpp
** File description:
** .
*/

#pragma once

#include "../../shared/Networking/ProtocolUtils.hpp"
#include "../../shared/Utilities/Timer.hpp"
#include "../../shared/Utilities/UUID.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "../../shared/Utilities/secureAsio.hpp"
#include <map>
#include <optional>
#include <string>
#include <vector>

struct Connection {
    asio::ip::address addr;
    asio::ip::port_type port;
    Utilities::UUID uuid;
    Timer timeoutTimer;
    int player;
    int lobby; // -1 is default and represents not in a lobby
};

class ConnectionManager {
public:
    // If serverUUID already generated, can be set in constructor
    ConnectionManager(Utilities::UUID serverUUID, int clientTimeout = 4);

    // Add new connection to vector and generates UUID for connection
    // If connection already exists, returns existing UUID for conn
    Utilities::UUID addConnection(asio::ip::address addr, asio::ip::port_type port);
    Utilities::UUID addConnection(asio::ip::address addr, asio::ip::port_type port, Utilities::UUID uuid);

    // Returns UUID for given connection
    // Returns empty string if connection doesn't exist
    std::optional<Utilities::UUID> getUUID(asio::ip::address addr, asio::ip::port_type port);
    bool uuidValid(Utilities::UUID uuid) const;

    std::optional<Connection> getConnection(Utilities::UUID uuid) const;
    std::optional<Connection> getConnection(asio::ip::address addr, asio::ip::port_type port);
    std::optional<Connection> getConnection(int clientId);

    // Remove connection
    void removeConnection(asio::ip::address addr, asio::ip::port_type port);
    void removeConnection(std::string uuid);

    // Reset timeout timer for specified client
    void resetTimeout(asio::ip::address addr, asio::ip::port_type port);
    // Reset timeout for all clients
    void resetTimeoutAll();
    // Remove all connections that timed out
    void removeDisconnected();

    // ─── Lobby Stuff ─────────────────────────────────────────────────────────────────────────

    void joinLobby(Utilities::UUID clientUUID, int lobbyID);
    std::vector<LobbyInfo> getLobbyInfos() const;

    // Get vector of clients in lobby
    std::vector<Connection> getLobbyConnections(int lobby);

    // Getters / Setters
    void setServerUUID(Utilities::UUID serverUUID);
    Utilities::UUID getServerUUID() const;
    int getConnectionCount() const;
    std::vector<Connection>& getConnections();

private:
    std::vector<Connection> _connections;
    Utilities::UUID _serverUUID;

    int _playerCounter = 0;
    int _clientTimeout;
};