
#include "UUID.hpp"

UUID::UUID() {
    uuid_generate_random(this->_uuid);
}

UUID::UUID(std::string uuidStr) {
    std::regex pattern("(\\w+-){4}\\w+");
    std::smatch match;
    std::regex_search(uuidStr, match, pattern);

    uuid_parse(match.str().c_str(), this->_uuid);
};

std::string UUID::toString() {
    char* uuidStr;

    uuid_unparse(this->_uuid, uuidStr);

    return std::string(uuidStr);
};

bool UUID::operator==(const UUID& uuid) {
    return uuid._uuid == this->_uuid;
}

std::string UUID::operator+(std::string str) {
    return this->toString() + str;
};

std::string UUID::operator+(char* str) {
    return this->toString() + str;
};