/*
** EPITECH PROJECT, 2022
** Utilities.hpp
** File description:
** .
*/

#pragma once

#include <string>
#include <vector>

class Utilities {
public:
    Utilities();
    ~Utilities();

    static std::vector<std::string> splitStr(std::string str, std::string sep) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> output;

        while ((pos = str.find(sep)) && pos != std::string::npos) {
            output.push_back(str.substr(0, pos));
            str.erase(0, pos + sep.length());
        }
        output.push_back(str);
        return output;
    }
};
