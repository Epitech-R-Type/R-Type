
#include "UUID.hpp"

// SoleUUID::SoleUUID() {
//     this->_uuid = sole::uuid4();
// }

// SoleUUID::SoleUUID(std::string uuidStr) {
//     std::regex pattern("(\\w+-){4}\\w+");
//     std::smatch match;
//     std::regex_search(uuidStr, match, pattern);

//     this->_uuid = sole::rebuild(match.str());
// };

// std::string SoleUUID::toString() {
//     std::stringstream ss;

//     ss << this->_uuid;
//     return ss.str();
// };

// bool SoleUUID::operator==(const SoleUUID& uuid) {
//     return uuid._uuid == this->_uuid;
// }

// std::string SoleUUID::operator+(std::string str) {
//     return this->toString() + str;
// };

// std::string SoleUUID::operator+(char* str) {
//     return this->toString() + str;
// };