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

// Command enum
enum Command {
    Here, // Server commands start here
    GetEntity,
    ActShoot,
    ActMove,
    Entity, // Client commands start here
    DeleteEntity,
    DeleteComponent,
};

// Parsed command structure$
struct ParsedCmd {
    Command cmd;
    std::vector<std::vector<std::string>> args;
};

class ProtocolUtils {
public:
    static std::optional<ParsedCmd> parseCommand(Message<std::string> msg) {
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

    static Message<std::string> createMessage(std::string cmd, std::string args, asio::ip::address addr, asio::ip::port_type port) {
        std::string body = cmd;
        body += " " + args + "\r\n";

        return Message<std::string>(body, addr, port);
    }
};
