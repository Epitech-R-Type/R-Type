/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "ECS.hpp"
#include "Manager.hpp"
enum ComponentType { ARMOR, HEALTH, POSITION };

namespace Armor {
    struct data {
        int armor = 0;
    };

    std::string toString(Armor::data component) {
        std::stringstream ss;

        ss << component.armor << ";";
        return ss.str();
    }

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
        if (manager->hasComponent<Armor::data>(entityID)) {
            Armor::data* component = manager->getComponent<Armor::data>(entityID);
            component->armor = stoi(args[1]);
        } else {
            manager->addComp<Armor::data>(entityID, {stoi(args[1])});
        }
    }
} // namespace Armor

struct Health {
    int health = 0;
};

std::string toString(Health component) {
    std::stringstream ss;

    ss << component.health << ";";
    return ss.str();
}

struct Position {
    float xPos = 0;
    float yPos = 0;
};

std::string toString(Position component) {
    std::stringstream ss;

    ss << component.xPos << "," << component.yPos << ";";
    return ss.str();
}