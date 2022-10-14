#pragma once

// #include "sole.hpp"
#include "uuid.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

class UUID {
public:
    UUID();

    UUID(std::string uuidStr);

    std::string toString();

    bool operator==(const UUID& uuid);

    friend std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
        return os << uuid._uuid;
    };

    friend std::string& operator<<(std::string& str, const UUID& uuid) {
        std::stringstream ss;

        ss << str << uuid;
        str = ss.str();
        return str;
    }

    std::string operator+(std::string str);

    std::string operator+(char* str);

private:
    uuids::uuid _uuid;
};