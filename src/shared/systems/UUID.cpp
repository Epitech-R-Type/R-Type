
#include "UUID.hpp"

UUIDM::UUIDM() {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    this->_uuid = gen();
}

UUIDM::UUIDM(std::string uuidStr) {
    std::regex pattern("(\\w+-){4}\\w+");
    std::smatch match;
    std::regex_search(uuidStr, match, pattern);

    std::optional<uuids::uuid> potUUID = uuids::uuid::from_string(match.str());

    if (potUUID.has_value())
        this->_uuid = potUUID.value();
};

std::string UUIDM::toString() {
    std::stringstream ss;

    ss << this->_uuid;

    return ss.str();
};

bool UUIDM::isValid() {
    return !this->_uuid.is_nil();
}

bool UUIDM::operator==(const UUIDM& uuid) {
    return uuid._uuid == this->_uuid;
}

std::string UUIDM::operator+(std::string str) {
    return this->toString() + str;
};

std::string UUIDM::operator+(char* str) {
    return this->toString() + str;
};