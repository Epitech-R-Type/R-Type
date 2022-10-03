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
    _sock(_ctxt, asio::ip::udp::endpoint(asio::ip::udp::v6(), 3500)), _t(_ctxt, asio::chrono::milliseconds(10))
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

            int conn_id = this->getConnectionManager().getClientId(addr, port);

            this->push_message({conn_id, this->_buffer});

            //Reset buffer
            memset(this->_buffer, 0, 1024);

            // Call incoming handler again
            this->setup_incoming_handler();
        } else {
            //Reset buffer
            memset(this->_buffer, 0, 1024);
            
            std::cerr << "Error performing async_receive_from()" << std::endl;
            this->setup_incoming_handler();
        }
    });
}

void Communication::setup_outgoing_handler()
{
    this->_t = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(10));

    this->_t.async_wait([this](const asio::error_code &err) {
        if (err) {
            std::cout << "Error is : " << err.message() << std::endl;
            this->setup_outgoing_handler();
            return;
        }
        std::optional<Message> msg;
        char buffer[1024];

        if ((msg = this->pop_message())) {
            auto addr = this->getConnectionManager().getClientAddr(msg->client_id);

            if (!addr) {
                std::cerr << "Cannot send message, client not in ConnectionsManager" << std::endl;
                this->setup_outgoing_handler();
                return;
            }

            // Prepare buffer
            int len = msg->msg.length();
            strcpy(buffer, msg->msg.c_str());
            memset(&buffer[len], 0, 1024 - len);

            asio::ip::udp::endpoint target(addr->ip, addr->port);
            this->_sock.send_to(asio::buffer(buffer), target);
        }
        this->setup_outgoing_handler();
    });
}

void Communication::push_message(Message msg)
{
    this->_incomingMessages->push(msg);
}

std::optional<Message> Communication::pop_message(void)
{
    return this->_outgoingMessages->pop();
}

ConnectionManager &Communication::getConnectionManager()
{
    return this->_connections;
}

void Communication::run()
{
    this->_ctxt.run();
}
