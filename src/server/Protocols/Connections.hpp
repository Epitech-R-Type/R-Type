/*
** EPITECH PROJECT, 2022
** Connections.hpp
** File description:
** .
*/

#pragma once

#include <asio.hpp>
#include <optional>
#include <string>
#include <vector>

#include "../../shared/systems/UUID.hpp"

struct Connection {
    asio::ip::address addr;
    asio::ip::port_type port;
    UUID uuid;
};

class ConnectionManager {
public:
    // If serverUUID already generated, can be set in constructor
    ConnectionManager(UUID serverUUID);

    // Add new connection to vector and generates UUID for connection
    // If connection already exists, returns existing UUID for conn
    UUID addConnection(asio::ip::address addr, asio::ip::port_type port);

    // Returns UUID for given connection
    // Returns empty string if connection doesn't exist
    std::optional<UUID> getUUID(asio::ip::address addr, asio::ip::port_type port);
    bool connectionExists(asio::ip::address addr, asio::ip::port_type port);
    std::optional<Connection> getConnection(UUID uuid) const;

    // Remove connection
    void removeConnection(asio::ip::address addr, asio::ip::port_type port);
    void removeConnection(std::string uuid);

    // Getters / Setters
    void setServerUUID(UUID serverUUID);
    UUID getServerUUID() const;
    int getConnectionCount() const;
    std::vector<Connection> getConnections();

private:
    std::vector<Connection> _connections;
    UUID _serverUUID;
};