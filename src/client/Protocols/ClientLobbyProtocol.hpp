#pragma once

#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/AsioConstants.hpp"
#include "../../shared/Utilities/UUID.hpp"

class ClientLobbyProtocol {
public:
    ClientLobbyProtocol();

    void connect(std::string serverIP, int port = TCP_PORT);

    void startGame();

    bool isAuthenticated();

    void handleIncMessages();

    void handleUserCommands(std::string command);

    void sendMessage(std::string msgContent);

    bool isConnected();

    bool shouldGameStart();

    void saveAuthentication(std::string uuids);

    void sendStart();

private:
    UUIDM _serverUUID;
    UUIDM _clientUUID;
    // Tcp com thread stuff
    std::thread* _comThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;

    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    asio::ip::address _serverIP;
    asio::ip::port_type _serverPort = 0;

    bool _connected = false;
    bool _authenticated = false;
    bool _startGame = false;
};