/*
** EPITECH PROJECT, 2022
** GameProtocol.cpp
** File description:
** .
*/

#include "GameProtocol.hpp"

GameProtocol::GameProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming, std::shared_ptr<MessageQueue<Message<std::string>>> outgoing,
                           std::vector<Connection> connections, std::shared_ptr<ECSManager> entManager)
    : _incomingMQ(incoming),
      _outgoingMQ(outgoing),
      _expectedClients(connections),
      _entityManager(entManager) {}

bool GameProtocol::waitForClients() {
    int targetClientCount = this->_expectedClients.size();
    Timer timeout(CONNECTION_DELAY);
    std::optional<Message<std::string>> msg;

    LOG("Waiting for clients.");

    while (this->_connectedClients.size() < targetClientCount && !timeout.isExpired()) {
        while ((msg = this->_incomingMQ->pop())) {
            LOG("Received new message.");
            // Parse command
            auto parsedCmd = ProtocolUtils::parseCommand(*msg);

            // if command invalid continue
            if (!parsedCmd) {
                ERROR("Invalid command: " << msg.value());
                continue;
            }

            // Only check for Here cmd, any other command is ignored at this point in the server execution
            if (parsedCmd->cmd == Command::Here) {
                if (this->handleHere(*parsedCmd, msg->getAddr(), msg->getPort()))
                    this->_players.push_back(this->_connectedClients.size());
            }
        }
    }
    LOG("Done waiting for clients");
}

//
//
// COMMAND HANDLING
//
//

bool GameProtocol::handleHere(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    // Error handling
    if (cmd.args.size() != 1) {
        ERROR("Argument length is not 1.");
        return false;
    }
    if (cmd.args[0].size() != 1) {
        ERROR("Argument[0] length is not 1.");
        return false;
    }

    UUIDM candidate(cmd.args[0][0]);

    for (auto conn : this->_expectedClients) {
        LOG("Connection: " << conn.uuid << " Candiate " << candidate);
        if (conn.uuid == candidate) {
            this->_connectedClients.push_back({addr, port, candidate});
            break;
        }
    }

    LOG("New Client is here!");
    return true;
}

void GameProtocol::handleMove(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    int player = this->getPlayer(addr, port);

    // Error handling
    if (0 > player || cmd.args.size() != 1 || cmd.args[0].size() != 1) {
        ERROR("Command is invalid.");
        return;
    }

    // In system if player is dead, send del Ent again ? or say something at least
    // This in case client has missed message that his character is dead
    std::string direction;

    LOG("Player " << player << " is moving " << direction << ".");

    // Note: Prints are placeholder and should be replaced by call to adequate system
    if (direction == "UP")
        std::cout << "Client is moving UP" << std::endl;
    if (direction == "DOWN")
        std::cout << "Client is moving DOWN" << std::endl;
    if (direction == "LEFT")
        std::cout << "Client is moving LEFT" << std::endl;
    if (direction == "RIGHT")
        std::cout << "Client is moving RIGHT" << std::endl;
}

void GameProtocol::handleShoot(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    int player = this->getPlayer(addr, port);

    // Error handling
    if (0 > player || cmd.args.size()) {
        ERROR("Command is invalid.");
        return;
    }

    LOG("Player " << player << " has fired their weapon.");
}

void GameProtocol::handleGetEnt(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    EntityID id;

    // Error handling
    if (cmd.args.size() != 1 || cmd.args[0].size() != 1) {
        ERROR("Command is invalid.");
        return;
    }

    // Attempt conversion to long long
    try {
        id = std::stoll(cmd.args[0][0]);
    } catch (...) {
        ERROR("Failed to convert argument to EntityID.");
        return;
    }

    // Send entity
    this->sendEntity(id);
}

void GameProtocol::handleCommands() {
    std::optional<Message<std::string>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        auto parsed = ProtocolUtils::parseCommand(*msg);

        if (!parsed) {
            ERROR("Unable to parse message.");
            continue;
        }

        switch (parsed->cmd) {
            case GetEntity:
                this->handleGetEnt(*parsed, msg->getAddr(), msg->getPort());
                break;
            case ActShoot:
                this->handleShoot(*parsed, msg->getAddr(), msg->getPort());
                break;
            case ActMove:
                this->handleMove(*parsed, msg->getAddr(), msg->getPort());
            default:
                WARNING("Unhandled Command: " << parsed->cmd);
        }
    }
}

//
//
// COMMAND SENDING
//
//

// Sends to all connected clients
template <class... T>
void GameProtocol::sendEntity(EntityID id) const {
    if (!this->_entityManager->isValidID(id)) {
        ERROR("Entity ID is invalid: " << id);
        return;
    }

    std::string entitySerialization = Serialization::entityToString<T...>(id, this->_entityManager);
    LOG("Sending to Clients: ENTITY " << entitySerialization);
    for (auto conn : this->_connectedClients) {
        LOG("Send to " << conn.addr << " " << conn.port << std::endl);
        this->_outgoingMQ->push(ProtocolUtils::createMessage("ENTITY", entitySerialization, conn.addr, conn.port));
    }
}

// Sends to only one client
template <class... T>
void GameProtocol::sendEntity(EntityID id, asio::ip::address addr, asio::ip::port_type port) const {
    if (!this->_entityManager->isValidID(id)) {
        ERROR("Entity ID is invalid: " << id);
        return;
    }
    std::string entitySerialization = Serialization::entityToString<T...>(id, this->_entityManager);
    LOG("Sending to Client: ENTITY " << entitySerialization);
    this->_outgoingMQ->push(ProtocolUtils::createMessage("ENTITY", entitySerialization, addr, port));
}

void GameProtocol::sendDelEntity(EntityID id) const {
    // if (!this->_entityManager->isValidID(id)) {
    //     ERROR("Entity ID is invalid: " << id);
    //     return;
    // }

    std::stringstream ss;
    ss << id;
    LOG("Sending to Clients: DEL_ENT " << ss.str());
    for (auto conn : this->_connectedClients) {
        LOG("Send to " << conn.addr << " " << conn.port << std::endl);
        this->_outgoingMQ->push(ProtocolUtils::createMessage("DEL_ENT", ss.str(), conn.addr, conn.port));
    }
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id) const {
    std::stringstream ss;
    ss << id << ";" << getID<T>();

    LOG("Sending to Clients: DEL_COMP " << ss.str());
    for (auto conn : this->_connectedClients) {
        LOG("Send to " << conn.addr << " " << conn.port << std::endl);
        this->_outgoingMQ->push(ProtocolUtils::createMessage("DEL_COMP", ss.str(), conn.addr, conn.port));
    }
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id, Connection client) const {
    std::stringstream ss;
    ss << id << ";" << getID<T>();

    LOG("Sending to Clients: DEL_COMP " << ss.str());
    this->_outgoingMQ->push(ProtocolUtils::createMessage("DEL_COMP", ss.str(), client.addr, client.port));
}

//
//
// UTILITIES
//
//

int GameProtocol::getPlayer(asio::ip::address addr, asio::ip::port_type port) {
    int i;
    bool found = false;

    for (i = 0; i < this->_connectedClients.size(); i++)
        if (this->_connectedClients[i].addr == addr && this->_connectedClients[i].port == port) {
            found = true;
            break;
        }
    if (found)
        return i;
    else
        return -1;
}
