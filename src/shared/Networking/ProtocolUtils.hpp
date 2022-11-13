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
#define SIZE_HEADER 2
#define CMD 2
#define UUID_PIECE sizeof(UuidBuf)

// Command binary protocol value
#define UPDATE_ENTITY 1
#define DELETE_ENTITY 2
#define ACTION_MOVE 9
#define ACTION_SHOOT 10
#define CHANGE_MUSIC 4
#define HERE 7
#define PING 11
#define GAME_END 5
#define DEATH 6

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
        ByteBuf buf;
        std::optional<ParsedCmd> output = {{Command::Here, buf}};
        unsigned short command = ProtocolUtils::getCommand(msg.getMsg());

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
        unsigned short finalSize = CMD + SIZE_HEADER + data.size();
        final.resize(finalSize);

        // Move final size and command into buffer
        memcpy(&final[0], &finalSize, SIZE_HEADER);
        memcpy(&final[SIZE_HEADER], &cmd, CMD);

        // Move data
        memcpy(&final[SIZE_HEADER + CMD], &data, data.size());

        auto output = Message<ByteBuf>(final, addr, port);

        return output;
    }

    // Read packet size on first two bytes of packet
    static unsigned short packetSize(ByteBuf buf) {
        if (buf.size() < 2)
            return buf.size();

        // Return unsigned short encoded in first two bytes
        return *((unsigned short*)&buf);
    }

    static unsigned short getCommand(ByteBuf buf) {
        if (buf.size() < 4)
            return 0;

        // Return unsigned short encoded in first two bytes
        return *((unsigned short*)&buf[2]);
    }

    // Convert char[] to ByteBuf
    static ByteBuf convertBuffer(char buffer[], size_t size) {
        ByteBuf output;

        for (int i = 0; i < size; i++)
            output.push_back(buffer[i]);

        return output;
    }
};
