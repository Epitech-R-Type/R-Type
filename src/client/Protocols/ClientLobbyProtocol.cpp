
#include "ClientLobbyProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "TcpClient.hpp"

ClientLobbyProtocol::ClientLobbyProtocol() {}

void ClientLobbyProtocol::connect(std::string serverIP, int port) {
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // Init tcp com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, serverIP, port);
}

void ClientLobbyProtocol::handleMessages() {

    while (!this->_incomingMQ->isEmpty()) {
        auto message = this->_incomingMQ->pop();

        if (this->_serverIP.is_unspecified()) {
            this->_serverIP = message->getAddr();
            this->_serverPort = message->getPort();
        }

        const std::string msg = message->getMsg();

        const std::vector<std::string> msgBits = Utilities::splitStr(msg, " ");

        if (msgBits[0] == "CONNECTED")
            this->_connected = true;

        // if (msgBits[0] == "START_GAME")
        //     this->_connected = true;
    }
}
