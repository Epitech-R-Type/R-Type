
#include "Entity.hpp"
#include "Components/Armor.hpp"
#include "Components/CollisionEffect.hpp"
#include "Components/Health.hpp"
#include "Components/Position.hpp"
#include <regex>
#include <sstream>

std::string Entity::serialize() const {
    std::string str("");
    bool first = true;

    for (const auto& component : this->_components) {
        if (!first)
            str += ',';
        else
            first = false;
        str += component.get()->serialize();
    }

    return str;
}

std::vector<int> buildArgsVect(std::string argsStr) {
    std::vector<int> args;
    std::string token;
    std::stringstream ss(argsStr);

    while (getline(ss, token, ',')) {
        args.push_back(stoi(token));
    }
    return args;
}

void Entity::deserialize(const std::string entity_string) {
    components type;

    std::regex component_names("([[:alpha:]]+)+", std::regex_constants::egrep);
    auto words_begin = std::sregex_iterator(entity_string.begin(), entity_string.end(), component_names);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        const std::string componentName = (*i).str();

        std::regex argsReg(std::string("") + componentName + "\\(([-\\d,]+?)\\)", std::regex_constants::ECMAScript);
        std::smatch match;

        std::regex_search(entity_string.begin(), entity_string.end(), match, argsReg);
        std::vector<int> args = buildArgsVect(match[1]);

        switch (type[componentName]) {
        case componentsEnum::HEALTH:
            this->updateComponent<Health>(args[0]);
            break;
        case componentsEnum::ARMOR:
            this->updateComponent<Armor>(args[0]);
            break;
        case componentsEnum::POSITION:
            this->updateComponent<Position>(args[0], args[1]);
            break;
        case componentsEnum::COLLISION:
            this->updateComponent<CollisionEffect>(args[0], args[1]);
            break;
        default:
            throw ECSError::InvalidComponent(componentName);
        }
    }
}
