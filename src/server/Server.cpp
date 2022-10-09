/*
** EPITECH PROJECT, 2022
** Server.cpp
** File description:
** .
*/

#include "Server.hpp"

Server::Server() {
    this->_incomingMsg = std::make_shared<MessageQueue<Message>>();
    this->_outgoingMsg = std::make_shared<MessageQueue<Message>>();
}

Server::~Server() {}

void Server::setup() {
    this->_comThread = std::thread(communication_main, this->_incomingMsg, this->_outgoingMsg);
}

void Server::main_loop() {
    std::optional<Message> msg;

    while (true) {
        while ((msg = this->_incomingMsg->pop())) {
            std::cout << msg->client_id << ": " << msg->msg;

            if (msg->msg == "ping\n") {
                std::cout << "[Server] -> [" << msg->client_id << "]: pong" << std::endl;
                this->_outgoingMsg->push({msg->client_id, "pong"});
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
