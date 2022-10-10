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
#include <optional>
#include <cstring>

#include <asio.hpp>

#include "../shared/MessageQueue/MessageQueue.hpp"

#define MAX_BUFFER_SIZE 1024

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

class Communication {
    public:
    Communication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);
    ~Communication();

    void setup_incoming_handler();
    void setup_outgoing_handler();

    void push_message(Message<std::string> msg);
    std::optional<Message<std::string>> pop_message(void);

    void run();

    private:
    asio::io_context _ctxt;
    asio::ip::udp::socket _sock;
    asio::ip::udp::endpoint _endpoint;

    char _buffer[MAX_BUFFER_SIZE];

    std::shared_ptr<MessageQueue<std::string>> _incomingMessages;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMessages;

    asio::steady_timer _t;
};
