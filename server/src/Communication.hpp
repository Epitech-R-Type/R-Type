/*
** EPITECH PROJECT, 2022
** Communication.hpp
** File description:
** .
*/

#pragma once

#include <string>
#include <memory>
#include <asio.hpp>

#include "../../shared/MessageQueue.hpp"

// Function passed to communication thread on creation
void communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);

class Communication {
    public:
        Communication(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing);
        ~Communication() = default;

        void setup_incoming_handler();
        void setup_outgoing_handler();
        void run();

    private:
        asio::io_context _ctxt;
        
        std::shared_ptr<MessageQueue<std::string>> _incomingMessages;
        std::shared_ptr<MessageQueue<std::string>> _outgoingMessages;
};
