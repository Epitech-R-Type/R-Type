/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "ECS.hpp"
#include <sstream>
#include <vector>

enum ComponentType { ARMOR, HEALTH, POSITION, ANIMATION };

namespace Armor {
    struct Component {
        int armor = 0;
    };

    std::string toString(Armor::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Armor

namespace Health {
    struct Component {
        int health = 0;
    };

    std::string toString(Health::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Health

namespace Position {
    struct Component {
        float xPos = 0;
        float yPos = 0;
    };

    std::string toString(Position::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Position

namespace Animation {
    enum AnimationID { Orb, Vortex, Cluster };
    struct Component {
        AnimationID animationID;
        unsigned long layer;
    };

    std::string toString(Animation::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Animation

namespace Velocity {
    struct Component {
        float xVelocity = 0;
        float yVelocity = 0;
    };

    std::string toString(Velocity::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Velocity