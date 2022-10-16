#pragma once

// #include "sole.hpp"
#include "uuid.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

class UUIDM {
public:
    /**
     * Generate a new UUID
     * */
    UUIDM();

    /**
     * Extract the first UUID of a string
     * */
    UUIDM(std::string uuidStr);

    /**
     * Cast UUID to string, to send it over network or similar
     * */
    std::string toString();

    bool isValid();

    bool operator==(const UUIDM& uuid);

    bool operator!=(const UUIDM& uuid);

    friend std::ostream& operator<<(std::ostream& os, const UUIDM& uuid) {
        return os << uuid._uuid;
    };

    friend std::string& operator<<(std::string& str, const UUIDM& uuid) {
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