/*
** EPITECH PROJECT, 2022
** Communication.hpp
** File description:
** .
*/

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include <asio.hpp>

#include "../../shared/MessageQueue.hpp"
#include "ConnectionManager.hpp"

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<Message>> incoming, std::shared_ptr<MessageQueue<Message>> outgoing);

class Communication {
    public:
        Communication(std::shared_ptr<MessageQueue<Message>> incoming, std::shared_ptr<MessageQueue<Message>> outgoing);
        ~Communication();

        void setup_incoming_handler();
        void setup_outgoing_handler();

        void push_message(Message msg);

        ConnectionManager &getConnectionManager();

        void run();

    private:
        asio::io_context _ctxt;
        asio::ip::udp::socket _sock;
        asio::ip::udp::endpoint _endpoint;
        
        char _buffer[1024];
        
        std::shared_ptr<MessageQueue<Message>> _incomingMessages;
        std::shared_ptr<MessageQueue<Message>> _outgoingMessages;

        ConnectionManager _connections;
};
