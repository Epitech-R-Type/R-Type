
#include "Entity.hpp"
#include "Components/Armor.hpp"
#include "Components/CollisionEffect.hpp"
#include "Components/Health.hpp"
#include "Components/Position.hpp"

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

void Entity::deserialize(std::string entity_string) {
    components type;

    std::regex component_names("([[:alpha:]]+)+", std::regex_constants::egrep);
    auto words_begin = std::sregex_iterator(entity_string.begin(), entity_string.end(), component_names);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        const std::string componentName = (*i).str();
        std::cout << "componentName: " << componentName << std::endl;
        switch (type[componentName]) {
        case componentsEnum::HEALTH:
            this->updateComponent<Health>(1);
            break;
        case componentsEnum::ARMOR:
            this->updateComponent<Armor>(1);
            break;
        case componentsEnum::POSITION:
            this->updateComponent<Position>(2, 1);
            break;
        case componentsEnum::COLLISION:
            this->updateComponent<CollisionEffect>();
            break;
        default:
            throw ECSError::InvalidComponent(componentName);
        }
    }
}
