/*
** EPITECH PROJECT, 2022
** ClientGameProtocol.cpp
** File description:
** .
*/

#include "ClientGameProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"

ClientGameProtocol::ClientGameProtocol(std::shared_ptr<MessageQueue<Message<std::string>>> incoming,
                                       std::shared_ptr<MessageQueue<Message<std::string>>> outgoing, std::shared_ptr<ECSManager> entManager,
                                       asio::ip::address addr, asio::ip::port_type port, UUIDM uuid)
    : _incomingMQ(incoming),
      _outgoingMQ(outgoing),
      _entityManager(entManager),
      _addr(addr),
      _port(port),
      _uuid(uuid) {
    LOG("UDP Sending to port : " << port);
}

//
//
// COMMAND HANDLING
//
//

void ClientGameProtocol::handleEntity(ParsedCmd cmd, std::string raw) {
    if (cmd.args.size() < 1)
        return;

    LOG("Creating Entity " << id);

    std::vector<std::string> res = Utilities::splitStr(raw, " ");
    Serialization::stringToEntity(res[1], this->_entityManager);
}

void ClientGameProtocol::handleDeleteEntity(ParsedCmd cmd) {
    EntityID id;

    if (cmd.args.size() != 1) {
        ERROR("Command " << cmd.cmd << " has not exactly one arg.");
        return;
    }

    try {
        id = std::stoll(cmd.args[0][0]);
    } catch (...) {
        ERROR("Unable to convert argument to long long.");
        return;
    }

    this->_entityManager->deleteEntity(id);
}

void ClientGameProtocol::handleDeleteComponent(ParsedCmd cmd) {
    EntityID id;
    Index compId;

    if (cmd.args.size() != 2) {
        ERROR("Command " << cmd.cmd << " has not exactly two args.");
        return;
    }

    try {
        id = std::stoll(cmd.args[0][0]);
        compId = std::stol(cmd.args[1][0]);
    } catch (...) {
        ERROR("Unable to convert arguments.");
        return;
    }

    LOG("Deleting Component " << compId << " of entity " << id);
    this->_entityManager->removeComp(id, compId);
}

bool ClientGameProtocol::handleCommands() {
    std::optional<Message<std::string>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        auto parsed = ProtocolUtils::parseCommand(*msg);

        if (!parsed)
            continue;

        switch (parsed->cmd) {
            case Entityd:
                this->handleEntity(parsed.value(), msg->getMsg());
                break;
            case DeleteEntity:
                this->handleDeleteEntity(parsed.value());
                break;
            case DeleteComponent:
                this->handleDeleteComponent(parsed.value());
                break;
            default:
                WARNING("Command " << parsed->cmd << " unhandled.");
        }
    }
    return true;
}

//
//
// COMMAND SENDING
//
//

void ClientGameProtocol::sendActMove(Move direction) {
    std::string body;

    switch (direction) {
        case UP:
            body = "UP";
            break;
        case DOWN:
            body = "DOWN";
            break;
        case LEFT:
            body = "LEFT";
            break;
        case RIGHT:
            body = "RIGHT";
            break;
    }

    auto msg = ProtocolUtils::createMessage("ACT_MOVE", body, this->_addr, this->_port);

    // LOG("Sending to Server: " << msg.getMsg());

    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendActFire() {
    auto msg = ProtocolUtils::createMessage("ACT_SHOOT", "", this->_addr, this->_port);
    // LOG("Sending to Server: " << msg.getMsg());
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendHere() {
    auto msg = ProtocolUtils::createMessage("HERE", this->_uuid.toString(), this->_addr, this->_port);
    // LOG("Sending to Server: " << msg.getMsg());
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendGetEnt(EntityID id) {
    std::stringstream ss;
    ss << id;

    auto msg = ProtocolUtils::createMessage("GET_ENT", ss.str(), this->_addr, this->_port);
    // LOG("Sending to Server: " << msg.getMsg());
    this->_outgoingMQ->push(msg);
}
