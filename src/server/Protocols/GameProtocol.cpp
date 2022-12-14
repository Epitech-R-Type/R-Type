/*
** EPITECH PROJECT, 2022
** GameProtocol.cpp
** File description:
** .
*/

#include "GameProtocol.hpp"
#include "../Systems/ArmamentSystem.hpp"
#include <iostream>

GameProtocol::GameProtocol(std::shared_ptr<MessageQueue<Message<ByteBuf>>> incoming, std::shared_ptr<MessageQueue<Message<ByteBuf>>> outgoing,
                           std::vector<Connection> connections, std::shared_ptr<ECSManager> entManager, Utilities::UUID serverUUID)
    : _incomingMQ(incoming),
      _outgoingMQ(outgoing),
      _expectedClients(connections),
      _entityManager(entManager),
      _connMan(serverUUID) {}

bool GameProtocol::waitForClients() {
    int targetClientCount = this->_expectedClients.size();
    Timer timeout(CONNECTION_DELAY);
    std::optional<Message<ByteBuf>> msg;

    while (this->_connMan.getConnectionCount() < targetClientCount && !timeout.isExpired()) {
        while ((msg = this->_incomingMQ->pop())) {
            // Parse command
            LOG((char*)&msg->getMsg()[0]);
            auto parsedCmd = ProtocolUtils::parseCommand(*msg);

            // if command invalid continue
            if (!parsedCmd) {
                ERRORLOG("Invalid command: " << parsedCmd->cmd);
                continue;
            }

            // Only check for Here cmd, any other command is ignored at this point in the server execution
            if (parsedCmd->cmd == Command::Here) {
                this->handleHere(*parsedCmd, msg->getAddr(), msg->getPort());
            }
        }
    }
    LOG("Done waiting for clients");

    return true;
}

// ─── Command Handling ────────────────────────────────────────────────────────────────────────────

bool GameProtocol::handleHere(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    // Error handling
    if (cmd.data.size() != UUID_PIECE) {
        LOG("UUID PIECE SIZE IS : " << UUID_PIECE);
        ERRORLOG("Argument length is not incorrect, expected " << UUID_PIECE << " got " << cmd.data.size());
        return false;
    }

    std::string uuidStr(cmd.data.begin(), cmd.data.end());

    Utilities::UUID candidate(uuidStr);

    for (auto conn : this->_expectedClients)
        if (conn.uuid == candidate) {
            this->_connMan.addConnection(addr, port, candidate);
            LOG("New Client is here!");
            break;
        }

    return true;
}

void GameProtocol::handleMove(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    int playerUID = this->getPlayer(addr, port);

    LOG("Handling move");

    // Error handling
    if (0 > playerUID || cmd.data.size() != 1) {
        ERRORLOG("Command is invalid.");
        return;
    }

    // In system if player is dead, send del Ent again ? or say something at least
    // This in case client has missed message that his character is dead
    char direction;
    memcpy(&direction, &cmd.data[0], DIRECTION_PIECE);

    EntityID entityID = INVALID_INDEX;
    for (auto beg = this->_entityManager->begin<Player::Component>(); beg != this->_entityManager->end<Player::Component>(); ++beg) {
        Player::Component* playerComp = this->_entityManager->getComponent<Player::Component>(*beg);

        if (playerComp->uniqueID == playerUID) {
            entityID = *beg;
            break;
        }
    }

    if (entityID == INVALID_INDEX)
        return;

    DEBUG("Player " << playerUID << " is moving " << direction << ".");

    Position::Component* position = this->_entityManager->getComponent<Position::Component>(entityID);
    Velocity::Component* velocity = this->_entityManager->getComponent<Velocity::Component>(entityID);

    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - velocity->timer;

    if (elapsed_seconds.count() > velocity->tickrate) {
        // Note: Prints are placeholder and should be replaced by call to adequate system
        switch (direction) {
            case Move::UP:
                position->y -= velocity->y;
                break;
            case Move::DOWN:
                position->y += velocity->y;
                break;
            case Move::LEFT:
                position->x -= velocity->x;
                break;
            case Move::RIGHT:
                position->x += velocity->x;
                break;
            case Move::UP + Move::RIGHT:
                position->x += (velocity->x + velocity->y) / 3;
                position->y -= (velocity->x + velocity->y) / 3;
                break;
            case Move::UP + Move::LEFT:
                position->x -= (velocity->x + velocity->y) / 3;
                position->y -= (velocity->x + velocity->y) / 3;
                break;
            case Move::DOWN + Move::RIGHT:
                position->x += (velocity->x + velocity->y) / 3;
                position->y += (velocity->x + velocity->y) / 3;
                break;
            case Move::DOWN + Move::LEFT:
                position->x -= (velocity->x + velocity->y) / 3;
                position->y += (velocity->x + velocity->y) / 3;
                break;
        };
        this->_entityManager->pushModified(entityID);
        velocity->timer = getNow();
    }
}

void GameProtocol::handleShoot(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    int playerUID = this->getPlayer(addr, port);
    LOG("Handling shoot");

    // Error handling
    if (0 > playerUID) {
        ERRORLOG("Command is invalid.");
        return;
    }

    EntityID entityID = INVALID_INDEX;
    for (auto beg = this->_entityManager->begin<Player::Component>(); beg != this->_entityManager->end<Player::Component>(); ++beg) {
        Player::Component* playerComp = this->_entityManager->getComponent<Player::Component>(*beg);

        if (playerComp->uniqueID == playerUID) {
            entityID = *beg;
            break;
        }
    }

    if (entityID == INVALID_INDEX)
        return;

    ArmamentSystem::makeWeapon(entityID, this->_entityManager);
}

// void GameProtocol::handleGetEnt(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
//     EntityID id;

//     // Error handling
//     if (cmd.args.size() != 1 || cmd.args[0].size() != 1) {
//         ERRORLOG("Command is invalid.");
//         return;
//     }

//     // Attempt conversion to long long
//     try {
//         id = std::stoll(cmd.args[0][0]);
//     } catch (...) {
//         ERRORLOG("Failed to convert argument to EntityID.");
//         return;
//     }

//     // Send entity
//     this->sendEntity(id);
// }

void GameProtocol::handlePing(ParsedCmd cmd, asio::ip::address addr, asio::ip::port_type port) {
    int playerUID = this->getPlayer(addr, port);

    // Error handling
    if (0 > playerUID) {
        ERRORLOG("Command is invalid.");
        return;
    }

    this->_connMan.resetTimeout(addr, port);
}

void GameProtocol::handleCommands() {
    std::optional<Message<ByteBuf>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        auto parsed = ProtocolUtils::parseCommand(*msg);

        if (!parsed) {
            ERRORLOG("Unable to parse message.");
            continue;
        }

        switch (parsed->cmd) {
            // case Command::GetEntity:
            //     this->handleGetEnt(*parsed, msg->getAddr(), msg->getPort());
            //     break;
            case Command::ActShoot:
                this->handleShoot(*parsed, msg->getAddr(), msg->getPort());
                break;
            case Command::ActMove:
                this->handleMove(*parsed, msg->getAddr(), msg->getPort());
                break;
            case Command::Ping:
                this->handlePing(*parsed, msg->getAddr(), msg->getPort());
                LOG("Handling ping");
                break;
            default:
                WARNING("Unhandled GameProtocol Command: " << parsed->cmd);
        }
    }
}

//─── Command Sending ─────────────────────────────────────────────────────────────────────────────

// Sends to all connected clients

void GameProtocol::sendEntity(EntityID id) {
    if (!this->_entityManager->isValidEntity(id)) {
        ERRORLOG("Entity ID is invalid: " << id);
        return;
    }

    ByteBuf entitySerialization = Serialization::entityToBuffer(id, this->_entityManager);
    for (auto conn : this->_connMan.getConnections())
        this->_outgoingMQ->push(ProtocolUtils::createMessage(UPDATE_ENTITY, entitySerialization, conn.addr, conn.port));
}

// Sends to only one client

void GameProtocol::sendEntity(EntityID id, asio::ip::address addr, asio::ip::port_type port) const {
    if (!this->_entityManager->isValidEntity(id)) {
        ERRORLOG("Entity ID is invalid: " << id);
        return;
    }

    ByteBuf entitySerialization = Serialization::entityToBuffer(id, this->_entityManager);
    this->_outgoingMQ->push(ProtocolUtils::createMessage(UPDATE_ENTITY, entitySerialization, addr, port));
}

void GameProtocol::sendDelEntity(EntityID id) {
    ByteBuf msgBody(ENTID_PIECE);
    memcpy(&msgBody[0], &id, ENTID_PIECE);

    for (auto conn : this->_connMan.getConnections())
        this->_outgoingMQ->push(ProtocolUtils::createMessage(DELETE_ENTITY, msgBody, conn.addr, conn.port));
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id) {
    ByteBuf msgBody(ENTID_PIECE + COMPID_PIECE);
    unsigned short compId = getID<T>();

    memcpy(&msgBody[0], &id, ENTID_PIECE);
    memcpy(&msgBody[ENTID_PIECE], &compId, COMPID_PIECE);

    for (auto conn : this->_connMan.getConnections()) {
        this->_outgoingMQ->push(ProtocolUtils::createMessage(DELETE_COMPONENT, msgBody, conn.addr, conn.port));
    }
}

template <class T>
void GameProtocol::sendDelComponent(EntityID id, Connection client) const {
    ByteBuf msgBody(ENTID_PIECE + COMPID_PIECE);
    unsigned short compId = getID<T>();

    memcpy(&msgBody[0], &id, 4);
    memcpy(&msgBody[ENTID_PIECE], &compId, 2);

    this->_outgoingMQ->push(ProtocolUtils::createMessage(DELETE_COMPONENT, msgBody, client.addr, client.port));
}

void GameProtocol::sendChangeMusic(int songId) {
    unsigned char id = songId;
    ByteBuf msgBody;
    msgBody.resize(1);
    memcpy(&id, &msgBody[0], 1);

    for (auto conn : this->_connMan.getConnections())
        this->_outgoingMQ->push(ProtocolUtils::createMessage(CHANGE_MUSIC, msgBody, conn.addr, conn.port));
}

void GameProtocol::sendGameEnd() {
    for (auto conn : this->_connMan.getConnections())
        this->_outgoingMQ->push(ProtocolUtils::createMessage(GAME_END, {}, conn.addr, conn.port));
}

void GameProtocol::sendDeath(asio::ip::address addr, asio::ip::port_type port) {
    this->_outgoingMQ->push(ProtocolUtils::createMessage(DEATH, {}, addr, port));
}

void GameProtocol::sendDeath(int clientId) {
    auto conn = this->_connMan.getConnection(clientId);

    if (!conn)
        return;

    this->_outgoingMQ->push(ProtocolUtils::createMessage(DEATH, {}, conn->addr, conn->port));
}

// ─── Utilities ───────────────────────────────────────────────────────────────────────────────────

int GameProtocol::getPlayer(asio::ip::address addr, asio::ip::port_type port) {
    int i;
    std::optional<Connection> conn;

    if ((conn = this->_connMan.getConnection(addr, port)))
        return conn->player;
    else
        return -1;
}

std::vector<Connection> GameProtocol::getConnectedClients() {
    return this->_connMan.getConnections();
}

void GameProtocol::handleDisconnectedClients() {
    auto connections = this->_connMan.getConnections();

    // Loop and check if connection is timed out
    for (auto conn : connections) {
        if (conn.timeoutTimer.isExpired()) {
            // If connection is timed out, remove entity associated to player
            for (auto beg = this->_entityManager->begin<Player::Component>(); beg != this->_entityManager->end<Player::Component>(); ++beg) {
                auto comp = this->_entityManager->getComponent<Player::Component>(*beg);

                if (comp->uniqueID == conn.player) {
                    WARNING("Deleting player entity [" << comp->uniqueID << "]");
                    this->_entityManager->pushModified(*beg);
                    this->_entityManager->deleteEntity(*beg);
                    break;
                }
            }

            // Inform to no longer send inputs
            this->sendDeath(conn.addr, conn.port);
        }
    }

    // Flush disconnected clients from connMan
    this->_connMan.removeDisconnected();
}

void GameProtocol::resetAllTimeouts() {
    this->_connMan.resetTimeoutAll();
}