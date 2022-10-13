/*
** EPITECH PROJECT, 2022
** TcpCommunication.cpp
** File description:
** .
*/

#include "TcpCommunication.hpp"

void tcp_communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                            std::shared_ptr<std::atomic<bool>> stopFlag) {
    TcpCommunication com(incoming, outgoing, stopFlag);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    // com.setup_incoming_handler();
    com.setup_outgoing_handler();
    com.stop_signal_handler();
    com.setup_acceptor_handler();

    // Run asio context
    com.run();
}

TcpCommunication::TcpCommunication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                                   std::shared_ptr<std::atomic<bool>> stopFlag)
    : _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)),
      _acceptor(_ctxt, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), TCP_PORT + 1)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

void TcpCommunication::setup_incoming_handler(std::shared_ptr<asio::ip::tcp::socket> peer) {
    peer->async_receive(asio::buffer(this->_buffer), [this, peer](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = peer->remote_endpoint().address();
            auto port = peer->remote_endpoint().port();

            this->push_message(Message(std::string(this->_buffer), addr, port));

            // Reset buffer
            memset(this->_buffer, 0, 1024);

            // Reset incoming handler
            this->setup_incoming_handler(peer);
        } else {
            // Reset buffer
            memset(this->_buffer, 0, 1024);

            // Reset incoming handler
            this->setup_incoming_handler(peer);
        }
    });
}

void TcpCommunication::setup_outgoing_handler() {}

// This handler accepts new connections and adds them to the connected peers vector
void TcpCommunication::setup_acceptor_handler() {
    auto newPeer = std::make_shared<asio::ip::tcp::socket>(this->_ctxt);
    this->_peers.push_back(newPeer);

    this->_acceptor.async_accept(*newPeer, [this, newPeer](const asio::error_code& err) {
        if (!err) {
            auto addr = newPeer->remote_endpoint().address();
            auto port = newPeer->remote_endpoint().port();

            // Push special message indicating that new client had been connected
            this->push_message(Message(std::string("ACCEPT"), addr, port));

            this->setup_incoming_handler(newPeer);
        }
        this->setup_acceptor_handler();
    });
}

// Handler methods
void TcpCommunication::stop_signal_handler() {
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

// Context runtime functions
void TcpCommunication::run() {
    // Execute context
    this->_ctxt.run();
}

void TcpCommunication::stop() {
    // Stop context
    this->_ctxt.stop();
}

// Access Methods
void TcpCommunication::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<std::string>> TcpCommunication::pop_message(void) {
    return this->_outgoingMessages->pop();
}

bool TcpCommunication::getStopFlag() {
    return *this->_stopFlag;
}

void TcpCommunication::pushNewPeer(std::shared_ptr<asio::ip::tcp::socket> newPeer) {
    this->_peers.push_back(newPeer);
}