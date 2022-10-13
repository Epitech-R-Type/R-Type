
#include "UUID.hpp"

UUID::UUID() {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    this->_uuid = gen();
}

UUID::UUID(std::string uuidStr) {
    std::regex pattern("(\\w+-){4}\\w+");
    std::smatch match;
    std::regex_search(uuidStr, match, pattern);

    this->_uuid = uuids::uuid::from_string(match.str()).value();
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