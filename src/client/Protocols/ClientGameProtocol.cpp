/*
** EPITECH PROJECT, 2022
** ClientGameProtocol.cpp
** File description:
** .
*/

#include "ClientGameProtocol.hpp"
#include "../../shared/Utilities/Utilities.hpp"

ClientGameProtocol::ClientGameProtocol(std::shared_ptr<MessageQueue<Message<ByteBuf>>> incoming,
                                       std::shared_ptr<MessageQueue<Message<ByteBuf>>> outgoing, std::shared_ptr<ECSManager> entManager,
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

// void ClientGameProtocol::handleEntity(ParsedCmd cmd, std::string raw) {
//     static int count = 0;
//     if (cmd.args.size() < 1) {
//         ERRORLOG("Command " << cmd.cmd << " has no args.");
//         return;
//     }

//     std::vector<std::string> res = Utilities::splitStr(raw, " ");
//     EntityID newEntity = Serialization::stringToEntity(res[1], this->_entityManager);
//     if (this->_entityManager->hasComponent<SoundCreation::Component>(newEntity) &&
//         this->_entityManager->getComponent<SoundCreation::Component>(newEntity)->ID != SFXID::INVALID)
//         MusicSystem::SFXQueue.push(this->_entityManager->getComponent<SoundCreation::Component>(newEntity)->ID);
// }

void ClientGameProtocol::handleDeleteEntity(ParsedCmd cmd) {
    EntityID id;

    if (cmd.data.size() != ENTID_PIECE) {
        ERRORLOG("Command " << cmd.cmd << " doesn't have right data size");
        return;
    }
    memcpy(&id, &cmd.data[0], ENTID_PIECE);

    if (this->_entityManager->hasComponent<SoundDestruction::Component>(id) && this->_entityManager->hasComponent<Position::Component>(id) &&
        this->_entityManager->getComponent<Position::Component>(id)->x > 0 && this->_entityManager->getComponent<Position::Component>(id)->y > 0 &&
        this->_entityManager->getComponent<Position::Component>(id)->x < GetScreenWidth() &&
        this->_entityManager->getComponent<Position::Component>(id)->y < GetScreenHeight())
        MusicSystem::SFXQueue.push(this->_entityManager->getComponent<SoundDestruction::Component>(id)->ID);
    this->_entityManager->deleteEntity(id);
}

void ClientGameProtocol::handleMusic(ParsedCmd cmd) {
    unsigned char songId;

    if (cmd.data.size() != SONG_PIECE) {
        ERRORLOG("Command " << cmd.cmd << " has not exactly one arg.");
        return;
    }

    memcpy(&songId, &cmd.data[0], SONG_PIECE);

    this->_musicSystem->changeSong(SongID(songId));
}

void ClientGameProtocol::handleDeleteComponent(ParsedCmd cmd) {
    EntityID id;
    Index compId;

    if (cmd.data.size() != ENTID_PIECE + COMPID_PIECE) {
        ERRORLOG("Command " << cmd.cmd << " invalid size.");
        return;
    }

    memcpy(&id, &cmd.data[0], ENTID_PIECE);
    memcpy(&compId, &cmd.data[ENTID_PIECE], COMPID_PIECE);

    this->_entityManager->removeComp(id, compId);
}

bool ClientGameProtocol::handleCommands() {
    std::optional<Message<ByteBuf>> msg;

    while ((msg = this->_incomingMQ->pop())) {
        LOG((char*)&msg->getMsg()[0]);
        auto parsed = ProtocolUtils::parseCommand(*msg);

        if (!parsed)
            continue;

        switch (parsed->cmd) {
            // case Command::Entityd:
            //     this->handleEntity(*parsed, msg->getMsg());
            //     break;
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
            case Command::Death:
                this->_isAlive = false;
                LOG("Client is dead");
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

void ClientGameProtocol::sendActMove(unsigned char direction) {
    if (!this->_isAlive)
        return;

    ByteBuf msgBody(DIRECTION_PIECE);
    memcpy(&msgBody[0], &direction, DIRECTION_PIECE);

    auto msg = ProtocolUtils::createMessage(ACTION_MOVE, msgBody, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendActFire() {
    if (!this->_isAlive)
        return;

    auto msg = ProtocolUtils::createMessage(ACTION_SHOOT, {}, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendHere() {
    std::string uuidStr = this->_uuid.toString();

    LOG("Uuid str length = " << uuidStr.length() << " " << uuidStr);

    ByteBuf data;
    data.resize(UUID_PIECE);
    memcpy(&data[0], uuidStr.c_str(), UUID_PIECE);

    auto msg = ProtocolUtils::createMessage(HERE, data, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendGetEnt(EntityID id) {
    ByteBuf msgBody(ENTID_PIECE);
    memcpy(&msgBody[0], &id, ENTID_PIECE);

    auto msg = ProtocolUtils::createMessage(GET_ENTITY, msgBody, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}

void ClientGameProtocol::sendPing() {
    auto msg = ProtocolUtils::createMessage(PING, {}, this->_addr, this->_port);
    this->_outgoingMQ->push(msg);
}
