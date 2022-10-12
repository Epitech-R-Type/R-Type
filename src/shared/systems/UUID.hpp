#pragma once

#include "sole.hpp"
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

private:
    sole::uuid _uuid;
};