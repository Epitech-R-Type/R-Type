/*
** EPITECH PROJECT, 2022
** TcpServer.hpp
** File description:
** .
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include <asio.hpp>
#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/AsioConstants.hpp"

void tcp_communication_main(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                            std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, std::shared_ptr<std::atomic<bool>> stopFlag);

class TcpServer {
public:
    TcpServer(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
              std::shared_ptr<std::atomic<bool>> stopFlag);

    // Setup action accepting new connections on socket
    // Note: Needs to be called again in order to loop
    void setup_acceptor_handler();

    // Setup action receiving TCP messages
    // Note: Needs to be called again in order to loop
    void setup_incoming_handler(std::shared_ptr<asio::ip::tcp::socket> peer);

    // Setup action polling MQ for new messages to send
    // Note: Needs to be called again in order to loop
    void setup_outgoing_handler();

    // This function will use an asio steady timer async wait in order to signal
    // context to stop if needed
    // Note: Needs to be called again in order to loop
    void stop_signal_handler();

    // Searches through peers and returns shared ptr to peer socket
    std::shared_ptr<asio::ip::tcp::socket> findPeer(asio::ip::address addr, asio::ip::port_type port);
    // Removes peer from vector
    void remove_peer(std::shared_ptr<asio::ip::tcp::socket> peer);
    // Access methods required for use in the async operation lambdas
    void push_message(Message<std::string> msg);

    std::optional<Message<std::string>> pop_out_message(void);

    void push_out_message(Message<std::string> msg);

    bool getStopFlag();

    void pushNewPeer(std::shared_ptr<asio::ip::tcp::socket> newPeer);

    // Execute context
    void run();
    // Stop context
    void stop();

private:
    // Async context executed in com thread
    asio::io_context _ctxt;

    // Asio networking
    asio::ip::tcp::acceptor _acceptor;

    std::vector<std::shared_ptr<asio::ip::tcp::socket>> _peers; // Peers socket vector

    // Buffer used for msg reception
    char _buffer[MAX_BUFFER_SIZE];

    // Messaging queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMessages;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMessages;

    // Timers
    asio::steady_timer _outgoingTimer; // Check for outgoing msg interval timer
    asio::steady_timer _stopTimer;     // Check for stop interval timer

    // Stop flag
    std::shared_ptr<std::atomic<bool>> _stopFlag;
};
