
#include "ClientLobbyProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "TcpClient.hpp"

ClientLobbyProtocol::ClientLobbyProtocol(std::shared_ptr<std::atomic<bool>> stopFlag) {
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_stopFlag = stopFlag;
}

int ClientLobbyProtocol::connect(std::string serverIP, int port) {
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // Init tcp com thread
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, serverIP, port);

    // Wait to find out if connection succeeded or not
    std::optional<Message<std::string>> msg;
    while (!(msg = this->_incomingMQ->pop()))
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (msg->getMsg() == "CONN_FAILED")
        return 1;

    this->sendAuthenticate();

    this->_connected = true;
    return 0;
}

// ─── Message Handling ────────────────────────────────────────────────────────────────────────────

int ClientLobbyProtocol::handleIncMessages() {
    std::optional<Message<std::string>> potMsg;
    while ((potMsg = this->_incomingMQ->pop())) {
        auto message = potMsg.value();
        std::cout << message << std::endl;

        if (this->_serverIP.is_unspecified()) {
            this->_serverIP = message.getAddr();
            this->_serverPort = message.getPort();
        }

        if (message.getMsg() == "CONN_CLOSED")
            return 1;

        const std::string msg = message.getMsg();

        const std::vector<std::string> msgBits = Utilities::splitStr(msg, " ");

        if (Utilities::UUID(msg) != this->_serverUUID) {
            ERRORLOG("Couldn't authenticate Server.");
            continue;
        }

        if (msgBits[0] == "START") {
            this->serverUdpPort = std::stoi(Utilities::splitStr(msgBits[1], ";")[1]); // get port
            this->_startGame = true;
        }
    }
    return 0;
}

// ─── Response Handling Stuff ─────────────────────────────────────────────────────────────────────

TcpResponse ClientLobbyProtocol::awaitResponse() {
    std::optional<Message<std::string>> msg;
    bool isResponse = false;

    while (!isResponse) {
        msg = this->_incomingMQ->pop();

        // Sleep if no message yet
        if (!msg) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // Put back message if not a response
        if (!(isResponse = this->isResponse(msg->getMsg())))
            this->_incomingMQ->push(*msg);
    }

    return this->parseResponse(*msg);
}

TcpResponse ClientLobbyProtocol::parseResponse(Message<std::string> msg) {
    auto splitMsg = Utilities::splitStr(msg.getMsg(), " ");
    TcpResponse output;

    // Note: No error handling here, responses should be correct
    output.code = std::stoi(splitMsg[0]);
    output.body = splitMsg[1];

    return output;
}

bool ClientLobbyProtocol::isResponse(std::string msgBody) {
    auto splitMsg = Utilities::splitStr(msgBody, " ");

    try {
        std::stoi(splitMsg[0]);
        return true;
    } catch (...) {
        return false;
    }
}

// ─── Message Sending ─────────────────────────────────────────────────────────────────────────────

void ClientLobbyProtocol::sendStart() {
    std::stringstream ss;

    ss << "START " << this->_clientUUID;

    this->sendMessage(ss.str());

    // Handle resp
    TcpResponse resp = this->awaitResponse();

    if (resp.code != 200)
        ERRORLOG("Error starting game: " << resp.body);
}

bool ClientLobbyProtocol::sendJoinLobby(int lobby) {
    std::stringstream ss;

    // Send message
    ss << "JOIN_LOBBY " << this->_clientUUID << " " << lobby; // THIS NEEDS TO BE SEMI COLOM SEPARATED
    this->sendMessage(ss.str());

    // Handle response
    TcpResponse resp = this->awaitResponse();

    if (resp.code != 200) {
        ERRORLOG("Error joining lobby: " << resp.body);
        return false;
    } else {
        LOG("Succesfully join lobby " << lobby << ".");
        this->lobby = lobby;
        return true;
    }
}

void ClientLobbyProtocol::sendAuthenticate() {
    this->sendMessage("AUTHENTICATE\r\n");
    const TcpResponse res = this->awaitResponse();

    this->saveAuthentication(res.body);
    this->_authenticated = true;
}

std::vector<LobbyInfo> ClientLobbyProtocol::sendGetLobbies() {
    std::stringstream ss;
    std::vector<LobbyInfo> output;

    // Send request
    ss << "GET_LOBBIES " << this->_clientUUID;
    this->sendMessage(ss.str());

    // Handle resp
    TcpResponse resp = this->awaitResponse();

    if (resp.code != 200) {
        ERRORLOG(resp.code << resp.body);
        return output;
    }

    auto splitMsg = Utilities::splitStr(resp.body, ";");

    // Parse Lobbies
    for (auto& lobby : splitMsg) {
        auto subArgs = Utilities::splitStr(lobby, ",");

        if (subArgs.size() != 3)
            continue;
        try {
            int id = std::stoi(subArgs[0]);
            bool isRunning = std::stoi(subArgs[1]);
            int playerCount = std::stoi(subArgs[2]);

            output.push_back({id, isRunning, playerCount});
        } catch (...) {
            continue;
        }
    }

    return output;
}

void ClientLobbyProtocol::sendLeave() {
    std::stringstream ss;

    ss << "LEAVE " << this->_clientUUID;
    this->sendMessage(ss.str());

    TcpResponse resp = this->awaitResponse();

    if (resp.code != 200)
        return;
}

void ClientLobbyProtocol::sendMessage(std::string msgContent) {
    Message<std::string> message(msgContent, this->_serverIP, this->_serverPort);
    this->_outgoingMQ->push(message);
}

// ─── Utility Functions ───────────────────────────────────────────────────────────────────────────

void ClientLobbyProtocol::saveAuthentication(std::string uuids) {
    std::vector<std::string> splitstr = Utilities::splitStr(uuids, ";");

    Utilities::UUID serverUUID = Utilities::UUID(splitstr[0]);
    Utilities::UUID clientUUID = Utilities::UUID(splitstr[1]);

    if (!this->_serverUUID.isValid() || !this->_clientUUID.isValid()) {
        ERRORLOG("Unable to get auhtentication from message.");
        return;
    }

    this->_serverUUID = serverUUID;
    this->_clientUUID = clientUUID;
}

bool ClientLobbyProtocol::isConnected() {
    return this->_connected;
}

bool ClientLobbyProtocol::shouldGameStart() {
    return this->_startGame;
}

void ClientLobbyProtocol::resetStartGame() {
    this->_startGame = false;
    this->_serverPort = -1;
};

void ClientLobbyProtocol::startGame() {
    this->sendMessage("START\r\n");
};

Utilities::UUID ClientLobbyProtocol::getUUID() {
    return this->_clientUUID;
}

int ClientLobbyProtocol::getServerPort() {
    return this->serverUdpPort;
}

asio::ip::address ClientLobbyProtocol::getServerIp() {
    return this->_serverIP;
}

int ClientLobbyProtocol::getLobby() {
    return this->lobby;
}
