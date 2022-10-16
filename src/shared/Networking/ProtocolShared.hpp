/*
** EPITECH PROJECT, 2022
** ProtocolShared.hpp
** File description:
** .
*/

#pragma once

#include <string>
#include <vector>

// This header is used to store any small part of protocol implementation that
// may be used by both the server and the client

// Command enum
enum Command { Here };

// Parsed command structure$
struct ParsedCmd {
    Command cmd;
    std::vector<std::vector<std::string>> args;
};
