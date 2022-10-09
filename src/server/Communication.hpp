/*
** EPITECH PROJECT, 2022
** Communication.hpp
** File description:
** .
*/

#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <asio.hpp>

#include "../shared/MessageQueue/MessageQueue.hpp"
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
    std::optional<Message> pop_message(void);

    ConnectionManager& getConnectionManager();

    void run();

private:
    asio::io_context _ctxt;
    asio::ip::udp::socket _sock;
    asio::ip::udp::endpoint _endpoint;

    char _buffer[1024];

    std::shared_ptr<MessageQueue<Message>> _incomingMessages;
    std::shared_ptr<MessageQueue<Message>> _outgoingMessages;

    asio::steady_timer _t;

    ConnectionManager _connections;
};
