/*
** EPITECH PROJECT, 2022
** LobbyProtocol.cpp
** File description:
** .
*/

#include "LobbyProtocol.hpp"

LobbyProtocol::LobbyProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing)
    : _connMan(UUID()) {
    // Set Messaging queues
    this->_incomingMQ = incoming;
    this->_outgoingMQ = outgoing;
}

bool LobbyProtocol::isAuthenticated(std::string uuid) {
    auto conn = this->_connMan.getConnection(uuid);

    return conn ? true : false;
}

bool LobbyProtocol::handleCommands() {
    std::optional<Message<std::string>> msg;
    bool gameShouldStart = false;

    while ((msg = this->_incomingMQ->pop())) {
        // Get peer info
        auto addr = msg->getAddr();
        auto port = msg->getPort();
        auto msgBody = msg->getMsg();

        // Pase msg
        auto splitBody = Utilities::splitStr(msgBody, " ");

        // If invalid size error 500
        if (splitBody.size() != 3) {
            Message<std::string> msg("500 Wrong request\r\n", addr, port);
            this->_outgoingMQ->push(msg);
        }

        std::string cmd = splitBody[0];

        // CONNECT Command
        if (cmd == "CONNECT") {
            // Add to connection manager and get new uuid
            auto uuid = this->_connMan.addConnection(addr, port);

            // Form msg body
            std::string body = "200 ";
            body << this->_connMan.getServerUUID();
            body += ";";
            body << uuid;
            body += "\r\n";

            Message<std::string> msg(body, addr, port);

            this->_outgoingMQ->push(msg);
        }

        // START command
        if (cmd == "START") {
        }
    }
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