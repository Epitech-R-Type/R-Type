/*
** EPITECH PROJECT, 2022
** Communication.hpp
** File description:
** .
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include <atomic>
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
#include "AsioConstants.hpp"

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                        std::shared_ptr<std::atomic<bool>> stopFlag);

class UdpCommunication {
public:
    UdpCommunication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag);

    // Setup action receiving UDP packets
    // Note: Both of these need to be called again in order to maintain the loop goig
    void setup_incoming_handler();

    // Setup action polling MQ for new messages to send
    // Note: Both of these need to be called again in order to maintain the loop goig
    void setup_outgoing_handler();

    // This function will use an asio steady timer async wait in order to signal
    // context to stop if needed
    void stop_signal_handler();

    // Access methods required for use in the async operation lambdas
    void push_message(Message<std::string> msg);
    std::optional<Message<std::string>> pop_message(void);
    bool getStopFlag();

    // Execute context
    void run();
    // Stop context
    void stop();

private:
    // Async context executed in com thread
    asio::io_context _ctxt;

    // Asio networking
    asio::ip::udp::socket _sock;
    asio::ip::udp::endpoint _endpoint;

    // Buffer used for msg reception
    char _buffer[MAX_BUFFER_SIZE];

    // Messaging queues
    std::shared_ptr<MessageQueue<std::string>> _incomingMessages;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMessages;

    // Timers
    asio::steady_timer _outgoingTimer; // Check for outgoing msg interval timer
    asio::steady_timer _stopTimer;     // Check for stop interval timer

    // Stop flag
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
