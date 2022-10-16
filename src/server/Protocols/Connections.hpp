/*
** EPITECH PROJECT, 2022
** Connections.hpp
** File description:
** .
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include <asio.hpp>
#include <optional>
#include <string>
#include <vector>

#include "../../shared/Utilities/UUID.hpp"

struct Connection {
    asio::ip::address addr;
    asio::ip::port_type port;
    UUIDM uuid;
};

class ConnectionManager {
public:
    // If serverUUID already generated, can be set in constructor
    ConnectionManager(UUIDM serverUUID);

    // Add new connection to vector and generates UUID for connection
    // If connection already exists, returns existing UUID for conn
    UUIDM addConnection(asio::ip::address addr, asio::ip::port_type port);

    // Returns UUID for given connection
    // Returns empty string if connection doesn't exist
    std::optional<UUIDM> getUUID(asio::ip::address addr, asio::ip::port_type port);
    bool uuidValid(UUIDM uuid) const;
    std::optional<Connection> getConnection(UUIDM uuid) const;

    // Remove connection
    void removeConnection(asio::ip::address addr, asio::ip::port_type port);
    void removeConnection(std::string uuid);

    // Getters / Setters
    void setServerUUID(UUIDM serverUUID);
    UUIDM getServerUUID() const;
    int getConnectionCount() const;
    std::vector<Connection> getConnections();

private:
    std::vector<Connection> _connections;
    UUIDM _serverUUID;
};