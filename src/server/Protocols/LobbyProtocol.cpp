/*
** EPITECH PROJECT, 2022
** LobbyProtocol.cpp
** File description:
** .
*/

#include "LobbyProtocol.hpp"

LobbyProtocol::LobbyProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                             std::shared_ptr<MessageQueue<Message<std::string>>> outgoing)
    : _connMan(UUIDM()) {
    // Set Messaging queues
    this->_incomingMQ = incoming;
    this->_outgoingMQ = outgoing;
}

bool LobbyProtocol::isAuthenticated(std::string uuid) {
    auto conn = this->_connMan.getConnection(uuid);

    return conn ? true : false;
}

void LobbyProtocol::sendResponse(std::string code, std::string args, asio::ip::address addr, asio::ip::port_type port) {
    std::string body = code;

    body += " ";
    body << this->_connMan.getServerUUID();
    body += " ";
    body += args;
    body += "\r\n";

    Message<std::string> msg(body, addr, port);
    this->_outgoingMQ->push(msg);
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
        if (splitBody.size() < 1) {
            Message<std::string> msg("500 Wrong request\r\n", addr, port);
            this->_outgoingMQ->push(msg);
            continue;
        }

        // Retrieve main command
        std::string cmd = splitBody[0];

        // CONNECT Command
        if (cmd == "CONNECT\r\n") {
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
            continue;
        }

        // If invalid size error 500
        if (splitBody.size() < 2) {
            Message<std::string> msg("500 Wrong request\r\n", addr, port);
            this->_outgoingMQ->push(msg);
            continue;
        }

        UUIDM uuid(splitBody[1]); // Potential failure here

        if (!this->_connMan.uuidValid(uuid)) {
            this->sendResponse("401", "Forbidden", addr, port);
            continue;
        }

        // START command
        if (cmd == "START") {

            // Set boolean that game should start

            this->startGame();
            continue;
        }
    }

    return gameShouldStart;
}

// Server Commands
void LobbyProtocol::startGame() {
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