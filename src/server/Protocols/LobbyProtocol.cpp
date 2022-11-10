/*
** EPITECH PROJECT, 2022
** LobbyProtocol.cpp
** File description:
** .
*/

#include "LobbyProtocol.hpp"

LobbyProtocol::LobbyProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                             std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, Utilities::UUID serverUUID,
                             std::shared_ptr<std::vector<GameInfo>> gamesToLaunch)
    : _connMan(serverUUID) {
    // Set Messaging queues
    this->_incomingMQ = incoming;
    this->_outgoingMQ = outgoing;
    this->_gamesToLaunch = gamesToLaunch;
}

// Server Commands
void LobbyProtocol::startGame(int port) {
    // Get Connections
    auto connections = this->_connMan.getConnections();

    // Form message body
    std::stringstream msgBody;
    msgBody << START_GAME << SP << this->_connMan.getServerUUID() << ";" << port << SP << END;

    LOG("Sending start game command.");
    // Send START_GAME command to all connected clients
    for (auto conn : connections) {
        Message<std::string> msg(msgBody.str(), conn.addr, conn.port);

        this->_outgoingMQ->push(msg);
    }
}

// ─── Handle Commands And Respond ─────────────────────────────────────────────────────────────────

void LobbyProtocol::handleCommands() {
    std::optional<Message<std::string>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        // Get peer info
        auto addr = msg->getAddr();
        auto port = msg->getPort();
        auto msgBody = msg->getMsg();

        // Handle client disconnection
        if (msgBody == "CONN_CLOSED") {
            this->_connMan.removeConnection(addr, port);
            continue;
        }

        // Parse msg
        auto splitBody = Utilities::splitStr(msgBody, " ");

        // If invalid size error 500
        if (splitBody.size() < 1) {
            this->handleWrongRequest(msgBody, addr, port);
            continue;
        }

        // Retrieve main command
        std::string cmd = splitBody[0];

        // CONNECT Command
        if (cmd == "CONNECT\r\n") {
            this->handleConnect(addr, port);
            continue;
        }

        // If invalid size error 500
        if (splitBody.size() < 2) {
            this->handleWrongRequest(msgBody, addr, port);
            continue;
        }

        Utilities::UUID uuid(splitBody[1]); // Potential failure here

        if (!this->_connMan.uuidValid(uuid)) {
            this->handleForbidden(addr, port);
            continue;
        }

        // START command
        if (cmd == "START") {
            this->handleStart();
            continue;
        }
    }
}

void LobbyProtocol::handleWrongRequest(std::string msgBody, asio::ip::address addr, asio::ip::port_type port) {
    Message<std::string> msg("500 Wrong request\r\n", addr, port);
    this->_outgoingMQ->push(msg);
}

void LobbyProtocol::handleForbidden(asio::ip::address addr, asio::ip::port_type port) {
    this->sendResponse("401", "Forbidden", addr, port);
}

void LobbyProtocol::handleConnect(asio::ip::address addr, asio::ip::port_type port) {
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

void LobbyProtocol::handleStart() {
    int port = UDP_PORT;

    // Find an available port
    while (!Utilities::isPortAvailable(port))
        port++;

    LOG("Using port: " << port);

    // Send start game
    this->startGame(port);

    // Add game info to gamesToStart
    this->_gamesToLaunch->push_back({0, port});
}

// ─── Utility Functions ───────────────────────────────────────────────────────────────────────────

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

std::vector<Connection> LobbyProtocol::getConnections() {
    return this->_connMan.getConnections();
}