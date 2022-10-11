/*
** EPITECH PROJECT, 2022
** Communication.cpp
** File description:
** .
*/

#include "UdpCommunication.hpp"

void spawnUDPThread(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing) {
    std::thread t = std::thread(communication_main, incoming, outgoing);

    t.detach();
}

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing) {
    UdpCommunication com(incoming, outgoing);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    com.setup_incoming_handler();
    com.setup_outgoing_handler();

    // Run asio context
    com.run();
}

UdpCommunication::UdpCommunication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing)
    : _sock(_ctxt, asio::ip::udp::endpoint(asio::ip::udp::v6(), 3501)), _t(_ctxt, asio::chrono::milliseconds(10)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

// Handler Method
void UdpCommunication::setup_incoming_handler() {
    this->_sock.async_receive_from(asio::buffer(this->_buffer), this->_endpoint, [this](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = this->_endpoint.address();
            auto port = this->_endpoint.port();

            this->push_message(Message(std::string(this->_buffer), addr, port));

            // Reset buffer
            memset(this->_buffer, 0, 1024);

            // Call incoming handler again
            this->setup_incoming_handler();
        } else {
            // Reset buffer
            memset(this->_buffer, 0, 1024);

            std::cerr << "Error performing async_receive_from()" << std::endl;
            this->setup_incoming_handler();
        }
    });
}

// Handler Method
void UdpCommunication::setup_outgoing_handler() {
    this->_t = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(10));

    this->_t.async_wait([this](const asio::error_code& err) {
        if (err) {
            std::cout << "Error is : " << err.message() << std::endl;
            this->setup_outgoing_handler();
            return;
        }
        std::optional<Message<std::string>> msg;
        char buffer[1024];

        if ((msg = this->pop_message())) {

            std::string msgStr = msg->getMsg();

            // Prepare buffer
            int len = msgStr.length();
            strcpy(buffer, msgStr.c_str());
            memset(&buffer[len], 0, 1024 - len);

            asio::ip::udp::endpoint target(msg->getAddr(), msg->getPort());
            this->_sock.send_to(asio::buffer(buffer), target);
        }
        this->setup_outgoing_handler();
    });
}

// Access Methods
void UdpCommunication::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<std::string>> UdpCommunication::pop_message(void) {
    return this->_outgoingMessages->pop();
}

void UdpCommunication::run() {
    this->_ctxt.run();
}
