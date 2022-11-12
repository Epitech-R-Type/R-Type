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

// This header is used to store any small part of protocol implementation that
// may be used by both the server and the client

#define UPDATE_ENTITY "ENTITY"
#define DELETE_ENTITY "DEL_ENT"
#define ACTION_MOVE "ACT_MOVE"
#define ACTION_SHOOT "ACT_SHOOT"
#define CHANGE_MUSIC "CHANGE_MUSIC"
#define HERE "HERE"
#define PING "PING"
#define GAME_END "GAME_END"
#define DEATH "DEATH"

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
    std::vector<std::vector<std::string>> args;
};

// Lobby info sent in get lobbies command
struct LobbyInfo {
    int id;
    bool isRunning;
    int playerCount;
};

class ProtocolUtils {
public:
    static std::optional<ParsedCmd> parseCommand(Message<std::string> msg) {
        std::optional<ParsedCmd> output = {{Command::Here, std::vector<std::vector<std::string>>()}};
        std::vector<std::string> splitMsg = Utilities::splitStr(msg.getMsg(), " ");

        // Error handling
        if (splitMsg.size() != 2) {
            ERRORLOG("Message length not 2.");
            return {};
        }
        // Check for CRLF
        if (splitMsg[1][splitMsg[1].size() - 1] != '\n' || splitMsg[1][splitMsg[1].size() - 2] != '\r') {
            ERRORLOG("Carriage Return Line Feed missing.");
            return {};
        }

        splitMsg[1].erase(splitMsg[1].length() - 2, 2);

        // Get command type
        if (splitMsg[0] == HERE)
            output->cmd = Command::Here;
        else if (splitMsg[0] == UPDATE_ENTITY)
            output->cmd = Command::Entityd;
        else if (splitMsg[0] == ACTION_SHOOT)
            output->cmd = Command::ActShoot;
        else if (splitMsg[0] == ACTION_MOVE)
            output->cmd = Command::ActMove;
        else if (splitMsg[0] == DELETE_ENTITY)
            output->cmd = Command::DeleteEntity;
        else if (splitMsg[0] == CHANGE_MUSIC)
            output->cmd = Command::ChangeMusic;
        else if (splitMsg[0] == PING)
            output->cmd = Command::Ping;
        else if (splitMsg[0] == GAME_END)
            output->cmd = Command::GameEnd;
        else if (splitMsg[0] == DEATH)
            output->cmd = Command::Death;
        else {
            ERRORLOG("Unhandled Command: " << splitMsg[0]);
            return {};
        }

        // Split args up in args and subargs
        auto splitArgs = Utilities::splitStr(splitMsg[1], ";");
        for (auto arg : splitArgs)
            output->args.push_back(Utilities::splitStr(arg, ","));

        return output;
    }

    static Message<std::string> createMessage(std::string cmd, std::string args, asio::ip::address addr, asio::ip::port_type port) {
        std::string body = cmd;
        body += " " + args + "\r\n";

        return Message<std::string>(body, addr, port);
    }
};
