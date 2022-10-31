/*
** EPITECH PROJECT, 2022
** ClientGameProtocol.hpp
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
#include "../../shared/Networking/ProtocolUtils.hpp"
#include "../../shared/Utilities/UUID.hpp"
#include "../../shared/Utilities/Utilities.hpp"

class ClientGameProtocol {
public:
    ClientGameProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                       std::shared_ptr<ECSManager> entManager, asio::ip::address addr, asio::ip::port_type port, UUIDM uuid);

    // COMMAND HANDLING
    void handleEntity(ParsedCmd cmd, std::string raw);
    void handleDeleteEntity(ParsedCmd cmd);
    void handleDeleteComponent(ParsedCmd cmd);

    // Returns true if player died
    bool handleCommands();

    // COMMAND SENDING
    void sendActMove(std::string direction);
    void sendActFire();
    void sendHere();
    void sendGetEnt(EntityID id);

private:
    // Udp messaging queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    // Server info
    asio::ip::address _addr;
    asio::ip::port_type _port;

    // Entity manager
    std::shared_ptr<ECSManager> _entityManager;

    // Client UUID
    UUIDM _uuid;
};
