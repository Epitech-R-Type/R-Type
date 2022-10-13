
#include "UUID.hpp"

UUID::UUID() {
    this->_uuid = sole::uuid4();
}

UUID::UUID(std::string uuidStr) {
    std::regex pattern("(\\w+-){4}\\w+");
    std::smatch match;
    std::regex_search(uuidStr, match, pattern);

    this->_uuid = sole::rebuild(match.str());
};

std::string UUID::toString() {
    std::stringstream ss;

    ss << this->_uuid;
    return ss.str();
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