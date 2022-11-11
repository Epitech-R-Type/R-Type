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

void LobbyProtocol::startGame(int port, int lobby) {
    // Get Connections
    auto connections = this->_connMan.getLobbyConnections(lobby);

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

        // ─── Internal Messages Not Received From Client ──────────────────────────────────

        // Handle client disconnection
        if (msgBody == "CONN_CLOSED") {
            this->_connMan.removeConnection(addr, port);
            continue;
        }

        // ─── Unconnected Commands ────────────────────────────────────────────────────────

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

        // ─── Authenticated Commands ──────────────────────────────────────────────────────

        Utilities::UUID uuid(splitBody[1]); // Potential failure here

        if (!this->_connMan.uuidValid(uuid)) {
            this->handleForbidden(addr, port);
            continue;
        }

        // START command
        if (cmd == "START")
            this->handleStart(uuid, addr, port);

        // JOIN LOBBY
        if (cmd == "JOIN_LOBBY") {
            try {
                int lobby = std::stoi(splitBody[2]);

                this->handleJoinLobby(addr, port, uuid, lobby);
            } catch (...) {
                this->handleWrongRequest(msgBody, addr, port);
            }
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

void LobbyProtocol::handleJoinLobby(asio::ip::address addr, asio::ip::port_type port, Utilities::UUID clientUUID, int lobby) {
    this->_connMan.joinLobby(clientUUID, lobby);

    this->sendResponse("200", "Lobby Joined", addr, port);
}

void LobbyProtocol::handleStart(Utilities::UUID uuid, asio::ip::address addr, asio::ip::port_type port) {
    int gamePort = UDP_PORT;

    // Check if user is in lobby
    int lobby = this->_connMan.getConnection(uuid)->lobby;
    if (lobby == -1) {
        this->sendResponse("400", "Client is not currently in a lobby", addr, port);
        return;
    }

    if (this->isLobbyRunning(lobby)) {
        this->sendResponse("400", "Lobby is already running", addr, port);
        return;
    }

    // Find an available port
    while (!Utilities::isPortAvailable(gamePort))
        gamePort++;

    LOG("Starting game for lobby [" << lobby << "] Using port: " << gamePort);

    // Send OK response
    this->sendResponse("200", "Ok. Starting game", addr, port);

    // Send start game
    this->startGame(gamePort, lobby);

    // Add game info to gamesToStart
    this->_gamesToLaunch->push_back({lobby, gamePort});
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

// ─── Getters And Setters ─────────────────────────────────────────────────────────────────────────

ConnectionManager& LobbyProtocol::getConnectionManager() {
    return this->_connMan;
}

void LobbyProtocol::addRunningLobby(int lobby) {
    this->_runningLobbies.push_back(lobby);
}

void LobbyProtocol::removeRunningLobbies(int lobby) {
    for (int i = 0; i < this->_runningLobbies.size(); i++)
        if (this->_runningLobbies[i] == lobby)
            this->_runningLobbies.erase(this->_runningLobbies.begin() + i);
}

bool LobbyProtocol::isLobbyRunning(int lobby) {
    for (auto candidate : this->_runningLobbies)
        if (candidate == lobby)
            return true;
    return false;
}