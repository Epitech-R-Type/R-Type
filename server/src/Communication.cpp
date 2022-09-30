/*
** EPITECH PROJECT, 2022
** Communication.cpp
** File description:
** .
*/

#include "Communication.hpp"

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<Message>> incoming, std::shared_ptr<MessageQueue<Message>> outgoing) {
    Communication com(incoming, outgoing);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    com.setup_incoming_handler();
    com.setup_outgoing_handler();

    // Run asio context
    com.run();
}

Communication::Communication(std::shared_ptr<MessageQueue<Message>> incoming, std::shared_ptr<MessageQueue<Message>> outgoing):
    _sock(_ctxt, asio::ip::udp::endpoint(asio::ip::udp::v6(), 3500))
{
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

Communication::~Communication()
{
    
}

void Communication::setup_incoming_handler()
{
    this->_sock.async_receive_from(asio::buffer(this->_buffer), this->_endpoint, [this](const asio::error_code &err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = this->_endpoint.address();
            auto port = this->_endpoint.port();

            std::cout << "Message From: " << addr << ":" << port << std::endl;

            int conn_id = this->getConnectionManager().getClientId(addr, port);

            this->push_message({conn_id, this->_buffer});
            
            // Call incoming handler again
            this->setup_incoming_handler();
        } else {
            std::cerr << "Error performing async_receive_from()" << std::endl;
            this->setup_incoming_handler();
        }
    });
}

void Communication::setup_outgoing_handler()
{
    
}

void Communication::push_message(Message msg)
{
    this->_incomingMessages->push(msg);
}

ConnectionManager &Communication::getConnectionManager()
{
    return this->_connections;
}

void Communication::run()
{
    this->_ctxt.run();
}
