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
    struct Component {
        int armor = 0;
    };

    std::string toString(Armor::Component component) {
        std::stringstream ss;

        ss << component.armor << ";";
        return ss.str();
    }

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
        if (manager->hasComponent<Armor::Component>(entityID)) {
            Armor::Component* component = manager->getComponent<Armor::Component>(entityID);
            component->armor = stoi(args[1]);
        } else {
            manager->addComp<Armor::Component>(entityID, {stoi(args[1])});
        }
    }
} // namespace Armor

namespace Health {
    struct Component {
        int health = 0;
    };

    std::string toString(Health::Component component) {
        std::stringstream ss;

        ss << component.health << ";";
        return ss.str();
    }

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
        if (manager->hasComponent<Health::Component>(entityID)) {
            Health::Component* component = manager->getComponent<Health::Component>(entityID);
            component->health = stoi(args[1]);
        } else {
            manager->addComp<Health::Component>(entityID, {stoi(args[1])});
        }
    }
} // namespace Health

namespace Position {
    struct Component {
        float xPos = 0;
        float yPos = 0;
    };

    std::string toString(Position::Component component) {
        std::stringstream ss;

        ss << component.xPos << "," << component.yPos << ";";
        return ss.str();
    }

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
        if (manager->hasComponent<Position::Component>(entityID)) {
            Position::Component* component = manager->getComponent<Position::Component>(entityID);
            component->xPos = strtof(args[1].c_str(), nullptr);
            component->yPos = strtof(args[2].c_str(), nullptr);
        } else {
            manager->addComp<Position::Component>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
        }
    }
} // namespace Position
