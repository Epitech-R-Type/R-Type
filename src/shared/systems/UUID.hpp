#pragma once

// #include "sole.hpp"
#include <iostream>
#include <regex>
#include <uuid/uuid.h>

class UUID {
public:
    UUID();

    UUID(std::string uuidStr);

    std::string toString();

    bool operator==(const UUID& uuid);

    friend std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
        return os << uuid._uuid;
    };

    std::string operator+(std::string str);

    std::string operator+(char* str);

private:
    uuid_t _uuid;
};