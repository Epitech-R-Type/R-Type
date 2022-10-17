/*
** EPITECH PROJECT, 2022
** TcpCommunication.cpp
** File description:
** .
*/

#include "TcpServer.hpp"
#include "../../shared/Utilities/Utilities.hpp"

void tcp_communication_main(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                            std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, std::shared_ptr<std::atomic<bool>> stopFlag) {
    TcpServer com(incoming, outgoing, stopFlag);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    com.setup_outgoing_handler();
    com.stop_signal_handler();
    com.setup_acceptor_handler();

    // Run asio context
    com.run();
}

TcpServer::TcpServer(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag)
    : _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)),
      _acceptor(_ctxt, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), TCP_PORT)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

// Incoming Handler
void TcpServer::setup_incoming_handler(std::shared_ptr<asio::ip::tcp::socket> peer) {
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

            // Handle client disconnection
            if (err.value() == asio::error::eof)
                this->remove_peer(peer);

            // Reset incoming handler
            this->setup_incoming_handler(peer);
        }
    });
}

// Outgoing Handler
void TcpServer::setup_outgoing_handler() {
    this->_outgoingTimer = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL));

    this->_outgoingTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            std::cout << "Error is : " << err.message() << std::endl;
            this->setup_outgoing_handler();
            return;
        }
        std::optional<Message<std::string>> msg;
        char buffer[1024];

        while ((msg = this->pop_out_message())) {
            std::string msgStr = msg->getMsg();

            // Prepare buffer
            int len = msgStr.length();
            strcpy(buffer, msgStr.c_str());
            memset(&buffer[len], 0, 1024 - len);

            auto peer = this->findPeer(msg->getAddr(), msg->getPort());
            if (!peer) {
                std::cout << "Peer: " << msg->getAddr() << ":" << msg->getPort() << " didn't stay around to receive their message" << std::endl;
                break;
            }

            std::cout << "Sending: " << buffer << " to: " << msg->getAddr() << ":" << msg->getPort() << std::endl;

            peer->send(asio::buffer(buffer));
        }
        this->setup_outgoing_handler();
    });
}

// Accept Handler
// This handler accepts new connections and adds them to the connected peers vector
void TcpServer::setup_acceptor_handler() {
    auto newPeer = std::make_shared<asio::ip::tcp::socket>(this->_ctxt);
    this->_peers.push_back(newPeer);

    this->_acceptor.async_accept(*newPeer, [this, newPeer](const asio::error_code& err) {
        if (!err) {
            auto addr = newPeer->remote_endpoint().address();
            auto port = newPeer->remote_endpoint().port();

            this->push_out_message(Message<std::string>("CONNECTED\r\n", addr, port));
            std::cout << "Tcp Client info is :" << newPeer->local_endpoint().port() << std::endl;
            this->setup_incoming_handler(newPeer);
        }
        this->setup_acceptor_handler();
    });
}

// Handler methods
void TcpServer::stop_signal_handler() {
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

std::shared_ptr<asio::ip::tcp::socket> TcpServer::findPeer(asio::ip::address addr, asio::ip::port_type port) {
    for (auto peer : this->_peers) {
        if (!peer->is_open()) {
            ERROR("Invalid Peer");
            continue;
        }
        auto peerAddr = peer->remote_endpoint().address();
        auto peerPort = peer->remote_endpoint().port();

        if (peerAddr == addr && peerPort == port)
            return peer;
    }

    return std::shared_ptr<asio::ip::tcp::socket>();
}

void TcpServer::remove_peer(std::shared_ptr<asio::ip::tcp::socket> peer) {
    for (int i = 0; i < this->_peers.size(); i++) {
        auto peerAddr1 = peer->remote_endpoint().address();
        auto peerPort1 = peer->remote_endpoint().port();

        auto peerAddr2 = this->_peers[i]->remote_endpoint().address();
        auto peerPort2 = this->_peers[i]->remote_endpoint().port();

        if (peerAddr1 == peerAddr2 && peerPort1 == peerPort2) {
            this->_peers.erase(this->_peers.begin() + i);
            break;
        }
    }
}

// Context runtime functions
void TcpServer::run() {
    // Execute context
    this->_ctxt.run();
}

void TcpServer::stop() {
    // Stop context
    this->_ctxt.stop();
}

// Access Methods
void TcpServer::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

// Access Methods
std::optional<Message<std::string>> TcpServer::pop_out_message(void) {
    return this->_outgoingMessages->pop();
}

void TcpServer::push_out_message(Message<std::string> msg) {
    this->_outgoingMessages->push(msg);
}

bool TcpServer::getStopFlag() {
    return *this->_stopFlag;
}

void TcpServer::pushNewPeer(std::shared_ptr<asio::ip::tcp::socket> newPeer) {
    this->_peers.push_back(newPeer);
}