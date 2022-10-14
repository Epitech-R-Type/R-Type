#pragma once

// #include "sole.hpp"
#include "uuid.h"
#include <iostream>
#include <regex>

class UUID {
public:
    /**
     * Generate a new UUID
     * */
    UUID();

    /**
     * Extract the first UUID of a string
     * */
    UUID(std::string uuidStr);

    /**
     * Cast UUID to string, to send it over network or similar
     * */
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