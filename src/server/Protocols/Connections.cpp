/*
** EPITECH PROJECT, 2022
** Connections.cpp
** File description:
** .
*/

#include "Connections.hpp"

ConnectionManager::ConnectionManager(UUIDM serverUUID, int clientTimeout)
    : _clientTimeout(clientTimeout) {
    this->_serverUUID = serverUUID;
}

UUIDM ConnectionManager::addConnection(asio::ip::address addr, asio::ip::port_type port) {
    auto existingUUID = this->getUUID(addr, port);

    // If connection doesn't already exist
    if (!existingUUID) {
        // Generate new uuid
        UUIDM newUUID;

        // Push new connection
        this->_connections.push_back({addr, port, newUUID, Timer(this->_clientTimeout), this->_playerCounter++});
        return newUUID;
    } else { // Connection already exists, return existing uuid
        return *existingUUID;
    }
}

UUIDM ConnectionManager::addConnection(asio::ip::address addr, asio::ip::port_type port, UUIDM uuid) {
    auto existingUUID = this->getUUID(addr, port);

    // If connection doesn't already exist
    if (!existingUUID) {
        // Push new connection
        this->_connections.push_back({addr, port, uuid, Timer(this->_clientTimeout), this->_playerCounter++});
        return uuid;
    } else { // Connection already exists, return existing uuid
        return *existingUUID;
    }
}

std::optional<UUIDM> ConnectionManager::getUUID(asio::ip::address addr, asio::ip::port_type port) {
    for (auto conn : this->_connections)
        if (conn.addr == addr && conn.port == port)
            return std::optional(conn.uuid);

    return {};
}

bool ConnectionManager::uuidValid(UUIDM uuid) const {
    for (auto conn : this->_connections)
        if (conn.uuid == uuid)
            return true;
    return false;
}

std::optional<Connection> ConnectionManager::getConnection(UUIDM uuid) const {
    for (auto conn : this->_connections)
        if (conn.uuid == uuid)
            return std::optional(conn);
    return {};
}

std::optional<Connection> ConnectionManager::getConnection(asio::ip::address addr, asio::ip::port_type port) {
    for (auto& conn : this->_connections)
        if (conn.addr == addr && conn.port == port)
            return std::optional(conn);
    return {};
}

void ConnectionManager::removeConnection(asio::ip::address addr, asio::ip::port_type port) {
    for (int i = 0; i < this->_connections.size(); i++)
        if (this->_connections[i].addr == addr && this->_connections[i].port == port)
            this->_connections.erase(this->_connections.begin() + i);
}

void ConnectionManager::removeConnection(std::string uuid) {
    for (int i = 0; i < this->_connections.size(); i++)
        if (this->_connections[i].uuid == uuid)
            this->_connections.erase(this->_connections.begin() + i);
}

void ConnectionManager::resetTimeout(asio::ip::address addr, asio::ip::port_type port) {
    for (auto& conn : this->_connections)
        if (conn.addr == addr && conn.port == port)
            conn.timeoutTimer.resetTimer();
}

void ConnectionManager::resetTimeoutAll() {
    for (auto conn : this->_connections)
        conn.timeoutTimer.resetTimer();
}

void ConnectionManager::removeDisconnected() {
    for (int i = 0; i < this->_connections.size(); i++)
        if (this->_connections[i].timeoutTimer.isExpired())
            this->_connections.erase(this->_connections.begin() + i);
}

void ConnectionManager::setServerUUID(UUIDM serverUUID) {
    this->_serverUUID = serverUUID;
}

UUIDM ConnectionManager::getServerUUID() const {
    return this->_serverUUID;
}

int ConnectionManager::getConnectionCount() const {
    return this->_connections.size();
}

std::vector<Connection>& ConnectionManager::getConnections() {
    return this->_connections;
}
