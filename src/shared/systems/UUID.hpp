#pragma once

// #include "sole.hpp"
#include "uuid.h"
#include <iostream>
#include <regex>

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
    uuids::uuid _uuid;
};