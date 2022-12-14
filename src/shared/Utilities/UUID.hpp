#pragma once

// #include "sole.hpp"
#include "uuid.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

typedef uuids::span<const std::byte, 16L> UuidBuf;

namespace Utilities {
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
         * transform a buffer to a UUID
         * */
        UUID(UuidBuf uuidBuffer);

        /**
         * Cast UUID to string, to send it over network or similar
         * */
        std::string toString();

        UuidBuf toBuffer();

        bool isValid();

        bool operator==(const Utilities::UUID& uuid) const;

        bool operator!=(const Utilities::UUID& uuid) const;

        friend std::ostream& operator<<(std::ostream& os, const Utilities::UUID& uuid) {
            return os << uuid._uuid;
        };

        friend std::string& operator<<(std::string& str, const Utilities::UUID& uuid) {
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
} // namespace Utilities