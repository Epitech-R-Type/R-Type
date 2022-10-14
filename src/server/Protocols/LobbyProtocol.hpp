/*
** EPITECH PROJECT, 2022
** LobbyProtocol.hpp
** File description:
** .
*/

#pragma once

#include <asio.hpp>
#include <memory>

#include "../../shared/MessageQueue/MessageQueue.hpp"

class LobbyProtocol {
public:
    LobbyProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

    // Utility function to check if users UUID is valid
    bool isAuthenticated(std::string uuid);

private:
    // Messaging Queues
    std::shared_ptr<MessageQueue<std::string>> _incomingMQ;
    std::shared_ptr<MessageQueue<std::string>> _outgoingMQ;
};
