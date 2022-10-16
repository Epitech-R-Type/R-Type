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
                            std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, std::shared_ptr<std::atomic<bool>> stopFlag,
                            std::string ipv6, int port);

class TcpClient {
public:
    TcpClient(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
              std::shared_ptr<std::atomic<bool>> stopFlag);

    // Setup action receiving TCP messages
    // Note: Needs to be called again in order to loop
    void setupIncomingHandler();

    // Setup action polling MQ for new messages to send
    // Note: Needs to be called again in order to loop
    void setupOutgoingHandler();

    // This function will use an asio steady timer async wait in order to signal
    // context to stop if needed
    // Note: Needs to be called again in order to loop
    void stopSignalHandler();

    void connect(std::string serverIP, int port);

    // Access methods required for use in the async operation lambdas
    void push_message(Message<std::string> msg);

    std::optional<Message<std::string>> pop_message(void);

    bool getStopFlag();

    // Execute context
    void run();
    // Stop context
    void stop();

    std::shared_ptr<asio::ip::tcp::socket> _server;

private:
    // Async context executed in com thread
    asio::io_context _ctxt;

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
