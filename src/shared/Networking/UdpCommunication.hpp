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
#include <thread>
#include <vector>

#include <asio.hpp>

#include "../MessageQueue/MessageQueue.hpp"

#define MAX_BUFFER_SIZE 1024

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

class UdpCommunication {
    public:
    UdpCommunication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

    // Setup action receiving UDP packets
    // Note: Both of these need to be called again in order to maintain the loop goig
    void setup_incoming_handler();

    // Setup action polling MQ for new messages to send
    // Note: Both of these need to be called again in order to maintain the loop goig
    void setup_outgoing_handler();

    // Access methods required for use in the async operation lambdas
    void push_message(Message<std::string> msg);
    std::optional<Message<std::string>> pop_message(void);

    void run();

    // Creates thread and returns it
    // IMPORTANT: Thread returned is unjoined
    friend void spawnUDPThread(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

    private:
    asio::io_context _ctxt;
    asio::ip::udp::socket _sock;
    asio::ip::udp::endpoint _endpoint;

    char _buffer[MAX_BUFFER_SIZE];

    std::shared_ptr<MessageQueue<std::string>> _incomingMessages;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMessages;

    asio::steady_timer _t;
};

void spawnUDPThread(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);
