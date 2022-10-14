/*
** EPITECH PROJECT, 2022
** LobbyProtocol.cpp
** File description:
** .
*/

#include "LobbyProtocol.hpp"

LobbyProtocol::LobbyProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing) {
    // Set Messaging queues
    this->_incomingMQ = incoming;
    this->_outgoingMQ = outgoing;
}

bool LobbyProtocol::isAuthenticated(std::string uuid) {
    auto conn = this->_connMan.getConnection(uuid);

    return conn ? true : false;
}

// Server Commands
void LobbyProtocol::startGame() {
    // If less than 2 connections, do nothing
    if (this->_connMan.getConnectionCount() < 2)
        return;

    // Get Connections
    auto connections = this->_connMan.getConnections();

    // Form message body
    std::stringstream msgBody;
    msgBody << START_GAME << SP << this->_connMan.getServerUUID() << SP << EMPTY_ARGS << END;

    // Send START_GAME command to all connected clients
    for (auto conn : connections) {
        Message<std::string> msg(msgBody.str(), conn.addr, conn.port);

        this->_outgoingMQ->push(msg);
    }
}