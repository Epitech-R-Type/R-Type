/*
** EPITECH PROJECT, 2022
** TcpCommunication.cpp
** File description:
** .
*/

#include "TcpClient.hpp"
#include "../../shared/Utilities/Utilities.hpp"

void tcp_communication_main(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                            std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, std::shared_ptr<std::atomic<bool>> stopFlag,
                            std::string serverIP, int port) {
    TcpClient com(incoming, outgoing, stopFlag);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    if (com.connect(serverIP, port)) {
        incoming->push(Message(std::string("CONN_FAILED"), asio::ip::address(), asio::ip::port_type()));
        return;
    } else
        incoming->push(Message(std::string("CONN_SUCCESS"), asio::ip::address(), asio::ip::port_type()));
    com.setupOutgoingHandler();
    com.stopSignalHandler();
    // Run asio context
    com.run();
}

TcpClient::TcpClient(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag)
    : _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

void TcpClient::setupIncomingHandler() {
    this->_server->async_receive(asio::buffer(this->_buffer), [this](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = _server->remote_endpoint().address();
            auto port = _server->remote_endpoint().port();

            this->push_message(Message(std::string(this->_buffer), addr, port));

            // Reset incoming handler

            // Reset buffer
            memset(this->_buffer, 0, 1024);
            // Reset incoming handler
            this->setupIncomingHandler();
        } else {
            ERROR("Error TcpClient handler:" << err.message());

            // Reset buffer
            memset(this->_buffer, 0, 1024);

            // // Handle server disconnection
            // if (err.value() == asio::error::eof)
            //     this->remove_peer(server);

            // Reset incoming handler
            this->setupIncomingHandler();
        }
    });
}

// Outgoing Handler
void TcpClient::setupOutgoingHandler() {
    this->_outgoingTimer = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL));

    this->_outgoingTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            ERROR("Error TcpClient handler:" << err.message());
            this->setupOutgoingHandler();
            return;
        }
        std::optional<Message<std::string>> msg;
        char buffer[1024];

        while ((msg = this->pop_message())) {

            std::string msgStr = msg->getMsg();

            // Prepare buffer
            int len = msgStr.length();
            strcpy(buffer, msgStr.c_str());
            memset(&buffer[len], 0, 1024 - len);

            this->_server->send(asio::buffer(buffer));
        }
        this->setupOutgoingHandler();
    });
}

// Handler methods
void TcpClient::stopSignalHandler() {
    this->_stopTimer = asio::steady_timer(this->_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL));

    this->_stopTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            ERROR("Error TcpClient handler:" << err.message());
            this->stopSignalHandler();
            return;
        }

        if (this->getStopFlag())
            this->stop();

        // Re schedule stop signal handler
        this->stopSignalHandler();
    });
}

int TcpClient::connect(std::string serverIP, int port) {
    // socket creation
    this->_server = std::make_shared<asio::ip::tcp::socket>(asio::ip::tcp::socket(this->_ctxt));

    try {
        this->_server->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(serverIP), port));
    } catch (std::system_error const& e) {
        ERROR("Connection to server refused, exiting now...");
        return 1;
    }

    this->setupIncomingHandler();
    return 0;
}

// Context runtime functions
void TcpClient::run() {
    // Execute context
    this->_ctxt.run();
}

void TcpClient::stop() {
    // Stop context
    this->_ctxt.stop();
}

// Access Methods
void TcpClient::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<std::string>> TcpClient::pop_message(void) {
    return this->_outgoingMessages->pop();
}

bool TcpClient::getStopFlag() {
    return *this->_stopFlag;
}
