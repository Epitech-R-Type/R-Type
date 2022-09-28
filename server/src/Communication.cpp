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

Communication::Communication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing)
{
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

void Communication::setup_incoming_handler()
{
    
}

void Communication::setup_outgoing_handler()
{
    
}

void Communication::run()
{
    
}
