#pragma once

#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/AsioConstants.hpp"
#include "../../shared/Utilities/UUID.hpp"

class ClientLobbyProtocol {
public:
    ClientLobbyProtocol(std::shared_ptr<std::atomic<bool>> stopFlag);

    /**
     * Connects the client to the TCP socket of the server
     * */
    int connect(std::string serverIP, int port = TCP_PORT);

    void startGame();

    // ─── Message Handling ────────────────────────────────────────────────────────────────────

    int handleIncMessages();

    // ─── Message Sending ─────────────────────────────────────────────────────────────────────

    void sendStart();
    void sendMessage(std::string msgContent);

    // ─── Utility Functions ───────────────────────────────────────────────────────────────────

    void saveAuthentication(std::string uuids);
    bool isConnected();

    bool shouldGameStart();
    void resetStartGame();

    int getServerPort();
    UUIDM getUUID();
    asio::ip::address getServerIp();

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

    // udp server port
    int port;

    bool _connected = false;
    bool _authenticated = false;
    bool _startGame = false;
};
