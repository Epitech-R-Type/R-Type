/*
** EPITECH PROJECT, 2022
** GameProtocol.hpp
** File description:
** .
*/

#pragma once

#include <asio.hpp>
#include <exception>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

#include "../../shared/ECS/Manager.hpp"
#include "../../shared/ECS/Serialization.hpp"
#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Utilities/Timer.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "Connections.hpp"

#define CONNECTION_DELAY 20 // Time in seconds to wait for clients to connect
#define MIN_CLIENTS 2

// Command enum
enum Command { Here };

// Parsed command structure$
struct ParsedCmd {
    Command cmd;
    std::vector<std::vector<std::string>> args;
};

class GameProtocol {
public:
    // Constructor
    GameProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                 std::vector<Connection> connections, std::shared_ptr<Manager> entManager);

    // Blocks in this method until all clients have joined or timeout has been reached
    // Returns false if enough clients have joined in order to start game
    // Returns true if not enough clients have joined
    bool waitForClients();

    // COMMAND HANDLING
    bool handleHere(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port);
    void handleMove(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port);
    void handleShoot(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port);
    void handleGetEnt(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port);
    void handleGetComp(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port);

    // COMMAND SENDING
    // By default, commands do nothing if invalid
    template <class... T>
    void sendEntity(EntityID id) const;
    template <class... T>
    void sendEntity(EntityID id, asio::ip::address addr, asio::ip::port_type port) const;

    void sendDelEntity(EntityID id) const;

    template <class T>
    void sendDelComponent(EntityID id) const;
    template <class T>
    void sendDelComponent(EntityID id, Connection client) const;

    // UILITIES
    static std::optional<ParsedCmd> parseCommand(Message<std::string> msg);
    static Message<std::string> createMessage(std::string cmd, std::string args, asio::ip::address addr, asio::ip::port_type port);
    int getPlayer(asio::ip::address addr, asio::ip::port_type port);

private:
    // Udp messaging queues
    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;

    // Entity manager
    std::shared_ptr<Manager> _entityManager;

    // Client tracking
    std::vector<Connection> _connectedClients; // Addr info is udp here
    std::vector<Connection> _expectedClients;  // Note: the addr info in this vector are for tcp, NOT udp
    std::vector<int> _players;                 // Each player has distinct int id, index of conn in _connectedClients = index in _players
};
