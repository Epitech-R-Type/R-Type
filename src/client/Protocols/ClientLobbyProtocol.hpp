#pragma once

#include "../../shared/MessageQueue/MessageQueue.hpp"

class ClientLobbyProtocol {
public:
    ClientLobbyProtocol();

    void connect(std::string serverIP, int port = TCP_PORT);

    void startGame();

    bool isAuthenticated();

    void handleMessages();

    void sendMessage();

private:
    // Tcp com thread stuff
    std::thread* _comThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;

    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;
};
