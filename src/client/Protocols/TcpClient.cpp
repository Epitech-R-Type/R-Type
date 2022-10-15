/*
** EPITECH PROJECT, 2022
** TcpClient.cpp
** File description:
** .
*/

#include "TcpClient.hpp"

void tcp_client_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag, asio::ip::address addr, asio::ip::port_type port) {
    TcpClient com(incoming, outgoing, stopFlag, addr, port);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    // com.setup_incoming_handler();
    com.setup_outgoing_handler();
    com.stop_signal_handler();

    // Run asio context
    com.run();
}

TcpClient::TcpClient(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag, asio::ip::address addr, asio::ip::port_type port)
    : _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)),
      _sock(_ctxt) {
    this->_sock.connect(asio::ip::tcp::endpoint(addr, port)); // Note: NO ERROR HANDLING WHATSOEVER HERE
}

// Incoming Handler
void TcpClient::setup_incoming_handler() {
    this->_sock.async_receive(asio::buffer(this->_buffer), [this](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            this->push_message(Message(std::string(this->_buffer), this->_addr, this->_port));

            // Reset buffer
            memset(this->_buffer, 0, MAX_BUFFER_SIZE);

            // Reset incoming handler
            this->setup_incoming_handler();
        } else {
            // Reset buffer
            memset(this->_buffer, 0, MAX_BUFFER_SIZE);

            // Reset incoming handler
            this->setup_incoming_handler();
        }
    });
}

// Outgoing Handler
void TcpClient::setup_outgoing_handler() {
    this->_outgoingTimer = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL));

    this->_outgoingTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            std::cout << "Error is : " << err.message() << std::endl;
            this->setup_outgoing_handler();
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

            this->_sock.send(asio::buffer(buffer));
        }
        this->setup_outgoing_handler();
    });
}

// Handler methods
void TcpClient::stop_signal_handler() {
    this->_stopTimer = asio::steady_timer(this->_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL));

    this->_stopTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            std::cerr << "Error in stop_signal_handler(): " << err.message() << std::endl;
            this->stop_signal_handler();
            return;
        }

        if (this->getStopFlag())
            this->stop();

        // Re schedule stop signal handler
        this->stop_signal_handler();
    });
}

void TcpClient::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<std::string>> TcpClient::pop_message(void) {
    return this->_outgoingMessages->pop();
}

bool TcpClient::getStopFlag() {
    return *this->_stopFlag;
}

void TcpClient::run() {
    this->_ctxt.run();
}

void TcpClient::stop() {
    this->_ctxt.stop();
}