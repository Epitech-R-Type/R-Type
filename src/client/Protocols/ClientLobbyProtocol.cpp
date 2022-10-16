
#include "ClientLobbyProtocol.hpp"
#include "TcpClient.hpp"

ClientLobbyProtocol::ClientLobbyProtocol() {}

void ClientLobbyProtocol::connect(std::string serverIP, int port) {
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // For some reason initializer list initialization wasn't working
    // this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ);

    // Init tcp com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, serverIP, port);

    while (this->_incomingMQ->isEmpty()) {
    }

    auto message = this->_incomingMQ->pop();
    this->_outgoingMQ->push(Message<std::string>("CONNECT\r\n", message->getAddr(), message->getPort()));
}
