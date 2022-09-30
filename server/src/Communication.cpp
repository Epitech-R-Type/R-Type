/*
** EPITECH PROJECT, 2022
** Communication.cpp
** File description:
** .
*/

#include "Communication.hpp"

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing) {
    Communication com(incoming, outgoing);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    com.setup_incoming_handler();
    com.setup_outgoing_handler();

    // Run asio context
    com.run();
}

Communication::Communication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing):
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
            std::cout << "Received : " << this->_buffer << std::endl;
            std::cout << "Endpoint info : " << this->_endpoint.port() << std::endl;
            this->push_message(std::string(this->_buffer));
            
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

void Communication::push_message(std::string msg)
{
    this->_incomingMessages->push(msg);
}

void Communication::run()
{
    this->_ctxt.run();
}
