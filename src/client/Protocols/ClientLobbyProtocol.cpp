
#include "ClientLobbyProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "TcpClient.hpp"

ClientLobbyProtocol::ClientLobbyProtocol(std::shared_ptr<std::atomic<bool>> stopFlag) {
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

    this->sendMessage("CONNECT\r\n");
    this->_connected = true;
    return 0;
}

asio::ip::address ClientLobbyProtocol::getServerIp() {
    return this->_serverIP;
}

// ─── Message Handling ────────────────────────────────────────────────────────────────────────────

int ClientLobbyProtocol::handleIncMessages() {
    std::optional<Message<std::string>> potMsg;
    while ((potMsg = this->_incomingMQ->pop())) {
        auto message = potMsg.value();

        if (this->_serverIP.is_unspecified()) {
            this->_serverIP = message.getAddr();
            this->_serverPort = message.getPort();
        }

        if (message.getMsg() == "CONN_CLOSED")
            return 1;

        const std::string msg = message.getMsg();

        const std::vector<std::string> msgBits = Utilities::splitStr(msg, " ");

        if (msgBits[0] == "200" && !this->_authenticated) {
            this->saveAuthentication(msg);
            this->_authenticated = true;
        }

        if (UUIDM(msg) != this->_serverUUID) {
            ERROR("Couldn't authenticate Server.");
            continue;
        }

        if (msgBits[0] == "START") {
            this->port = std::stoi(Utilities::splitStr(msgBits[1], ";")[1]); // get port
            this->_startGame = true;
        }
    }
    return 0;
}

void ClientLobbyProtocol::handleUserCommands(std::string command) {

    if (command == "Start")
        this->sendMessage("START\r\n");
}

// ─── Message Sending ─────────────────────────────────────────────────────────────────────────────

void ClientLobbyProtocol::sendStart() {
    std::stringstream ss;

    ss << "START " << this->_clientUUID;

    this->sendMessage(ss.str());
}

void ClientLobbyProtocol::sendMessage(std::string msgContent) {
    Message<std::string> message(msgContent, this->_serverIP, this->_serverPort);
    this->_outgoingMQ->push(message);
}

// ─── Utility Functions ───────────────────────────────────────────────────────────────────────────
void ClientLobbyProtocol::saveAuthentication(std::string uuids) {
    std::vector<std::string> splitstr = Utilities::splitStr(uuids, ";");

    UUIDM serverUUID = UUIDM(splitstr[0]);
    UUIDM clientUUID = UUIDM(splitstr[1]);

    if (!this->_serverUUID.isValid() || !this->_clientUUID.isValid()) {
        ERROR("Unable to get auhtentication from message.");
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
};

int ClientLobbyProtocol::getServerPort() {
    return this->port;
}

UUIDM ClientLobbyProtocol::getUUID() {
    return this->_clientUUID;
}
