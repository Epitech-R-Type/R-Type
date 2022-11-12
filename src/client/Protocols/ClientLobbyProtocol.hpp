#pragma once

#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/AsioConstants.hpp"
#include "../../shared/Utilities/UUID.hpp"

struct TcpResponse {
    int code;
    std::string body;
};

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

    // Response handling stuff
    TcpResponse awaitResponse();
    static TcpResponse parseResponse(Message<std::string> msg);
    static bool isResponse(std::string msgBody);

    // ─── Message Sending ─────────────────────────────────────────────────────────────────────

    void sendStart();
    void sendMessage(std::string msgContent);
    void sendJoinLobby(int lobby);

    // ─── Utility Functions ───────────────────────────────────────────────────────────────────

    void saveAuthentication(std::string uuids);
    bool isConnected();
    bool shouldGameStart();
    void resetStartGame();

    Utilities::UUID getUUID();
    int getServerPort();
    asio::ip::address getServerIp();
    int getLobby();

private:
    Utilities::UUID _serverUUID;
    Utilities::UUID _clientUUID;

    // Tcp com thread stuff
    std::thread* _comThread;
    std::shared_ptr<std::atomic<bool>> _stopFlag;

    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    asio::ip::address _serverIP;
    asio::ip::port_type _serverPort = 0;

    // udp server port
    int serverUdpPort;

    bool _connected = false;
    bool _authenticated = false;
    bool _startGame = false;
    int lobby = -1; // -1 if not in lobby otherwise, lobby id
};
