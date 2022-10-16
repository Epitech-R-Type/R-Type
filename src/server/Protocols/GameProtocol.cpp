/*
** EPITECH PROJECT, 2022
** GameProtocol.cpp
** File description:
** .
*/

#include "GameProtocol.hpp"

GameProtocol::GameProtocol(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                           std::vector<Connection> connections, std::shared_ptr<Manager> entManager)
    : _incomingMQ(incoming),
      _outgoingMQ(outgoing),
      _expectedClients(connections),
      _entityManager(entManager) {}

bool GameProtocol::waitForClients() {
    int targetClientCount = this->_expectedClients.size();
    Timer timeout(CONNECTION_DELAY);
    std::optional<Message<std::string>> msg;

    while (this->_connectedClients.size() < targetClientCount && !timeout.isExpired()) {
        while ((msg = this->_incomingMQ->pop())) {
            // Parse command
            auto parsedCmd = this->parseCommand(*msg);

            // if command invalid continue
            if (!parsedCmd)
                continue;

            // Only check for Here cmd
            if (parsedCmd->cmd == Command::Here)
                this->handleHere(*parsedCmd, msg->getAddr(), msg->getPort());
        }
    }
}

//
//
// COMMAND HANDLING
//
//

void GameProtocol::handleHere(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    // Error handling
    if (cmd.args.size() != 1)
        return;
    if (cmd.args[0].size() != 1)
        return;

    UUID candidate(cmd.args[0][1]);
    for (auto conn : this->_expectedClients)
        if (conn.uuid == candidate) {
            this->_connectedClients.push_back({addr, port, candidate});
            break;
        }
}

//
//
// COMMAND SENDING
//
//

// Sends to all connected clients
void GameProtocol::sendNewEntity(EntityID id) const {
    if (!this->_entityManager->isValidID(id))
        return;
    std::string entitySerialization = entityToString<>(id, this->_entityManager.get());

    for (auto conn : this->_connectedClients)
        this->_outgoingMQ->push(this->createMessage("ENTITY", entitySerialization, conn.addr, conn.port));
}

// Sends to only one client
void GameProtocol::sendNewEntity(EntityID id, Connection client) const {
    if (!this->_entityManager->isValidID(id))
        return;
    std::string entitySerialization = entityToString<>(id, this->_entityManager.get());
    this->_outgoingMQ->push(this->createMessage("ENTITY", entitySerialization, client.addr, client.port));
}

void GameProtocol::sendDelEntity(EntityID id) const {
    std::stringstream ss;
    ss << id;

    for (auto conn : this->_connectedClients)
        this->_outgoingMQ->push(this->createMessage("DEL_ENT", ss.str(), conn.addr, conn.port));
}

template <class T>
void GameProtocol::sendComponent(EntityID id) const {
    if (!this->_entityManager->isValidComp(id, getID<T>()))
        return;
    std::stringstream ss;

    ss << id;
    ss << ";" << componentToString<T>(id, this->_entityManager.get());

    for (auto conn : this->_connectedClients)
        this->_outgoingMQ->push(this->createMessage("COMPONENT", ss.str(), conn.addr, conn.port));
}

template <class T>
void GameProtocol::sendComponent(EntityID id, Connection client) const {
    if (!this->_entityManager->isValidComp(id, getID<T>()))
        return;
    std::stringstream ss;

    ss << id;
    ss << ";" << componentToString<T>(id, this->_entityManager.get());
    this->_outgoingMQ->push(this->createMessage("COMPONENT", ss.str(), client.addr, client.port));
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id) const {
    std::stringstream ss;
    ss << id << ";" << getID<T>();

    for (auto conn : this->_connectedClients)
        this->_outgoingMQ->push(this->createMessage("DEL_COMP", ss.str(), conn.addr, conn.port));
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id, Connection client) const {
    std::stringstream ss;
    ss << id << ";" << getID<T>();

    this->_outgoingMQ->push(this->createMessage("DEL_COMP", ss.str(), client.addr, client.port));
}

//
//
// UTILITIES
//
//

std::optional<ParsedCmd> GameProtocol::parseCommand(Message<std::string> msg) {
    std::optional<ParsedCmd> output;
    auto splitMsg = Utilities::splitStr(msg.getMsg(), " ");

    // Error handling
    if (splitMsg.size() != 2)
        return {};
    // Check for CRLF
    if (splitMsg[1][splitMsg.size() - 1] != '\n' || splitMsg[1][splitMsg.size() - 2] != '\r')
        return {};
    splitMsg[1].erase(splitMsg[1].length() - 2, 2);

    // Get command type
    if (splitMsg[0] == "HERE")
        output->cmd = Command::Here; // Note make sure you do ELSE IF
    else
        return {};

    // Split args up in args and subargs
    auto splitArgs = Utilities::splitStr(splitMsg[1], ";");
    for (auto arg : splitArgs)
        output->args.push_back(Utilities::splitStr(arg, ","));

    return output;
}

Message<std::string> GameProtocol::createMessage(std::string cmd, std::string args, asio::ip::address addr, asio::ip::port_type port) {
    std::string body = cmd;
    body += " " + args + "\r\n";

    return Message<std::string>(body, addr, port);
}
