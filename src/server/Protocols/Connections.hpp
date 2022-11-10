/*
** EPITECH PROJECT, 2022
** Connections.hpp
** File description:
** .
*/

#pragma once

#include "../../shared/Utilities/secureAsio.hpp"
#include <optional>
#include <string>
#include <vector>

#include "../../shared/Utilities/UUID.hpp"

struct Connection {
    asio::ip::address addr;
    asio::ip::port_type port;
    Utilities::UUID uuid;
};

class ConnectionManager {
public:
    // If serverUUID already generated, can be set in constructor
    ConnectionManager(Utilities::UUID serverUUID);

    // Add new connection to vector and generates UUID for connection
    // If connection already exists, returns existing UUID for conn
    Utilities::UUID addConnection(asio::ip::address addr, asio::ip::port_type port);

    // Returns UUID for given connection
    // Returns empty string if connection doesn't exist
    std::optional<Utilities::UUID> getUUID(asio::ip::address addr, asio::ip::port_type port);
    bool uuidValid(Utilities::UUID uuid) const;
    std::optional<Connection> getConnection(Utilities::UUID uuid) const;

    // Remove connection
    void removeConnection(asio::ip::address addr, asio::ip::port_type port);
    void removeConnection(std::string uuid);

    // Getters / Setters
    void setServerUUID(Utilities::UUID serverUUID);
    Utilities::UUID getServerUUID() const;
    int getConnectionCount() const;
    std::vector<Connection> getConnections();

private:
    std::vector<Connection> _connections;
    Utilities::UUID _serverUUID;
};