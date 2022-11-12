/*
** EPITECH PROJECT, 2022
** ClientGameProtocol.hpp
** File description:
** .
*/

#pragma once

#include "../../shared/ECS/ECSManager.hpp"
#include "../../shared/ECS/Serialization.hpp"
#include "../../shared/MessageQueue/MessageQueue.hpp"
#include "../../shared/Networking/ProtocolUtils.hpp"
#include "../../shared/Utilities/UUID.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "../../shared/Utilities/secureAsio.hpp"
#include "../Systems/MusicSystem.hpp"
#include <exception>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

class ClientGameProtocol {
public:
    ClientGameProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                       std::shared_ptr<ECSManager> entManager, std::shared_ptr<MusicSystem> musicSystem, asio::ip::address addr,
                       asio::ip::port_type port, Utilities::UUID uuid);

    // COMMAND HANDLING
    void handleEntity(ParsedCmd cmd, std::string raw);
    void handleDeleteEntity(ParsedCmd cmd);
    void handleDeleteComponent(ParsedCmd cmd);
    void handleMusic(ParsedCmd cmd);

    // Returns true if player died
    bool handleCommands();

    // COMMAND SENDING
    void sendActMove(std::string direction);
    void sendActFire();
    void sendHere();
    void sendGetEnt(EntityID id);
    void sendPing();

private:
    // Udp messaging queues
    std::shared_ptr<MessageQueue<Message<std::string>>> _incomingMQ;
    std::shared_ptr<MessageQueue<Message<std::string>>> _outgoingMQ;

    // Server info
    asio::ip::address _addr;
    asio::ip::port_type _port;

    // Entity manager
    std::shared_ptr<ECSManager> _entityManager;

    // Systems
    std::shared_ptr<MusicSystem> _musicSystem;

    // Client UUID
    Utilities::UUID _uuid;

    // is client alive
    bool _isAlive = true;
};
