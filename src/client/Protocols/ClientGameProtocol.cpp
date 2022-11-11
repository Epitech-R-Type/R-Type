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
                                       std::shared_ptr<MusicSystem> musicSystem, asio::ip::address addr, asio::ip::port_type port,
                                       Utilities::UUID uuid)
    : _incomingMQ(incoming),
      _outgoingMQ(outgoing),
      _entityManager(entManager),
      _musicSystem(musicSystem),
      _addr(addr),
      _port(port),
      _uuid(uuid) {}

// ─── Command Handling ────────────────────────────────────────────────────────────────────────────

void ClientGameProtocol::handleEntity(ParsedCmd cmd, std::string raw) {
    static int count = 0;
    if (cmd.args.size() < 1) {
        ERRORLOG("Command " << cmd.cmd << " has no args.");
        return;
    }

    std::vector<std::string> res = Utilities::splitStr(raw, " ");
    EntityID newEntity = Serialization::stringToEntity(res[1], this->_entityManager);
    if (this->_entityManager->hasComponent<SoundCreation::Component>(newEntity) &&
        this->_entityManager->getComponent<SoundCreation::Component>(newEntity)->ID != SFXID::INVALID)
        MusicSystem::SFXQueue.push(this->_entityManager->getComponent<SoundCreation::Component>(newEntity)->ID);
}

void ClientGameProtocol::handleDeleteEntity(ParsedCmd cmd) {
    EntityID id;

    if (cmd.args.size() != 1) {
        ERRORLOG("Command " << cmd.cmd << " has not exactly one arg.");
        return;
    }

    try {
        id = std::stoll(cmd.args[0][0]);
    } catch (...) {
        ERRORLOG("Unable to convert argument to long long.");
        return;
    }
    if (this->_entityManager->hasComponent<SoundDestruction::Component>(id) && this->_entityManager->hasComponent<Position::Component>(id) &&
        this->_entityManager->getComponent<Position::Component>(id)->x > 0 && this->_entityManager->getComponent<Position::Component>(id)->y > 0 &&
        this->_entityManager->getComponent<Position::Component>(id)->x < GetScreenWidth() &&
        this->_entityManager->getComponent<Position::Component>(id)->y < GetScreenHeight())
        MusicSystem::SFXQueue.push(this->_entityManager->getComponent<SoundDestruction::Component>(id)->ID);
    this->_entityManager->deleteEntity(id);
}

void ClientGameProtocol::handleMusic(ParsedCmd cmd) {
    int songId;

    if (cmd.args.size() != 1) {
        ERRORLOG("Command " << cmd.cmd << " has not exactly one arg.");
        return;
    }

    try {
        songId = std::stoi(cmd.args[0][0]);
    } catch (...) {
        ERRORLOG("Unable to convert argument to int.");
        return;
    }

    this->_musicSystem->changeSong(SongID(songId));
}

void ClientGameProtocol::handleDeleteComponent(ParsedCmd cmd) {
    EntityID id;
    Index compId;

    if (cmd.args.size() != 2) {
        ERRORLOG("Command " << cmd.cmd << " doesn't have two args.");
        return;
    }

    try {
        id = std::stoll(cmd.args[0][0]);
        compId = std::stol(cmd.args[1][0]);
    } catch (...) {
        ERRORLOG("Unable to convert arguments.");
        return;
    }

    this->_entityManager->removeComp(id, compId);
}

bool ClientGameProtocol::handleCommands() {
    std::optional<Message<std::string>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        auto parsed = ProtocolUtils::parseCommand(*msg);

        if (!parsed)
            continue;

        switch (parsed->cmd) {
            case Command::Entityd:
                this->handleEntity(*parsed, msg->getMsg());
                break;
            case Command::DeleteEntity:
                this->handleDeleteEntity(*parsed);
                break;
            case Command::DeleteComponent:
                this->handleDeleteComponent(*parsed);
                break;
            case Command::ChangeMusic:
#ifndef WIN32_LEAN_AND_MEAN
                this->handleMusic(*parsed);
#endif
                break;
            case Command::GameEnd:
                return true;
                break;
            default:
                WARNING("Command " << parsed->cmd << " unhandled.");
        }
    }
    return false;
}

// ─── Command Sending ─────────────────────────────────────────────────────────────────────────────

void ClientGameProtocol::sendActMove(std::string directions) {
    auto msg = ProtocolUtils::createMessage("ACT_MOVE", directions, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendActFire() {
    auto msg = ProtocolUtils::createMessage("ACT_SHOOT", "", this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendHere() {
    auto msg = ProtocolUtils::createMessage("HERE", this->_uuid.toString(), this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendGetEnt(EntityID id) {
    std::stringstream ss;
    ss << id;

    auto msg = ProtocolUtils::createMessage("GET_ENT", ss.str(), this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendPing() {
    auto msg = ProtocolUtils::createMessage("PING", "", this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}
