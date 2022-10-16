
#include "ClientLobbyProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "TcpClient.hpp"

ClientLobbyProtocol::ClientLobbyProtocol() {}

void ClientLobbyProtocol::connect(std::string serverIP, int port) {
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // Init tcp com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, serverIP, port);

    this->sendMessage("CONNECT\r\n");
    this->_connected = true;
}

void ClientLobbyProtocol::saveAuthentication(std::string uuids) {
    std::vector<std::string> splitstr = Utilities::splitStr(uuids, ";");

    this->_serverUUID = UUIDM(splitstr[0]);
    this->_clientUUID = UUIDM(splitstr[1]);

    std::cout << this->_clientUUID << " " << this->_serverUUID << std::endl;
}

void ClientLobbyProtocol::sendStart() {
    std::stringstream ss;

    ss << "START " << this->_clientUUID;

    this->sendMessage(ss.str());
}

void ClientLobbyProtocol::handleIncMessages() {
    std::optional<Message<std::string>> potMsg;
    while ((potMsg = this->_incomingMQ->pop())) {
        auto message = potMsg.value();

        if (this->_serverIP.is_unspecified()) {
            this->_serverIP = message.getAddr();
            this->_serverPort = message.getPort();
        }

        const std::string msg = message.getMsg();
        std::cout << "[Received] " << msg << std::endl;

        const std::vector<std::string> msgBits = Utilities::splitStr(msg, " ");

        if (msgBits[0] == "200" && !this->_authenticated) {
            this->saveAuthentication(msg);
            this->_authenticated = true;
        }

        if (UUIDM(msg) != this->_serverUUID) {
            std::cout << "Couldn't authenticate Server." << std::endl;
            continue;
        }

        if (msgBits[0] == "START")
            this->_startGame = true;
    }
}

void ClientLobbyProtocol::handleUserCommands(std::string command) {

    if (command == "Start")
        this->sendMessage("START\r\n");
}

void ClientLobbyProtocol::sendMessage(std::string msgContent) {
    Message<std::string> message(msgContent, this->_serverIP, this->_serverPort);
    this->_outgoingMQ->push(message);
}

bool ClientLobbyProtocol::isConnected() {
    return this->_connected;
}

bool ClientLobbyProtocol::shouldGameStart() {
    return this->_startGame;
};
