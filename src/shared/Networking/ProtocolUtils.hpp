/*
** EPITECH PROJECT, 2022
** ProtocolUtils.hpp
** File description:
** .
*/

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../MessageQueue/MessageQueue.hpp"
#include "../Utilities/Utilities.hpp"

typedef std::vector<char> ByteBuf;

// This header is used to store any small part of protocol implementation that
// may be used by both the server and the client

// Protocol piece sizes
#define SIZE_HEADER 8
#define CMD 2
#define UUID_PIECE 36
#define ENTID_PIECE 8
#define COMPID_PIECE 4
#define DIRECTION_PIECE 1
#define SONG_PIECE 1

// Command binary protocol value
#define UPDATE_ENTITY 1
#define DELETE_ENTITY 2
#define DELETE_COMPONENT 3
#define CHANGE_MUSIC 4
#define GAME_END 5
#define DEATH 6
#define HERE 7
#define GET_ENTITY 8
#define ACTION_MOVE 9
#define ACTION_SHOOT 10
#define PING 11

// Command enum
enum Command {
    Here, // Server commands start here
    GetEntity,
    ActShoot,
    ActMove,
    GameEnd,
    Death,
    Entityd, // Client commands start here
    DeleteEntity,
    DeleteComponent,
    ChangeMusic,
    Ping
};

// Parsed command structure$
struct ParsedCmd {
    Command cmd;
    ByteBuf data;
};

// Lobby info sent in get lobbies command
struct LobbyInfo {
    int id;
    bool isRunning;
    int playerCount;
};

class ProtocolUtils {
public:
    static std::optional<ParsedCmd> parseCommand(Message<ByteBuf> msg) {
        ByteBuf buf = msg.getMsg();
        unsigned short command = ProtocolUtils::getCommand(msg.getMsg());

        unsigned long size = ProtocolUtils::packetSize(msg.getMsg());

        buf.erase(buf.begin(), buf.begin() + SIZE_HEADER + CMD);
        buf.erase(buf.begin() + size - SIZE_HEADER - CMD, buf.end());

        std::optional<ParsedCmd> output = {{Command::Here, buf}};

        // Get command type
        if (command == HERE)
            output->cmd = Command::Here;
        else if (command == UPDATE_ENTITY)
            output->cmd = Command::Entityd;
        else if (command == ACTION_SHOOT)
            output->cmd = Command::ActShoot;
        else if (command == ACTION_MOVE)
            output->cmd = Command::ActMove;
        else if (command == DELETE_ENTITY)
            output->cmd = Command::DeleteEntity;
        else if (command == CHANGE_MUSIC)
            output->cmd = Command::ChangeMusic;
        else if (command == PING)
            output->cmd = Command::Ping;
        else if (command == GAME_END)
            output->cmd = Command::GameEnd;
        else if (command == DEATH)
            output->cmd = Command::Death;
        else {
            ERRORLOG("Unhandled Command: " << command);
            return {};
        }

        return output;
    }

    static Message<ByteBuf> createMessage(unsigned short cmd, ByteBuf data, asio::ip::address addr, asio::ip::port_type port) {
        ByteBuf final;

        std::cout << &data[0] << std::endl;

        const unsigned long finalSize = CMD + SIZE_HEADER + data.size();
        LOG("Final size is : " << data.size());
        final.resize(finalSize);

        // Move final size and command into buffer
        memcpy(&final[0], (char*)&finalSize, SIZE_HEADER);
        memcpy(&final[SIZE_HEADER], (char*)&cmd, CMD);

        // Move data
        memcpy(&final[SIZE_HEADER + CMD], (char*)&data[0], data.size());

        auto output = Message<ByteBuf>(final, addr, port);

        return output;
    }

    // Read packet size on first two bytes of packet
    static unsigned long packetSize(ByteBuf buf) {
        if (buf.size() < SIZE_HEADER)
            return buf.size();

        // Return unsigned short encoded in first two bytes
        return *((unsigned long*)&buf[0]);
    }

    static unsigned short getCommand(ByteBuf buf) {
        if (buf.size() < SIZE_HEADER + CMD)
            return 0;

        // Return unsigned short encoded in first two bytes
        return *((unsigned short*)&buf[SIZE_HEADER]);
    }

    // Convert char[] to ByteBuf
    static ByteBuf convertBuffer(char buffer[], size_t size) {
        ByteBuf output;

        for (int i = 0; i < size; i++)
            output.push_back(buffer[i]);

        return output;
    }
};
