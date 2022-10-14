/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "ECS.hpp"
#include <ctime>
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
        int maxHealth = 0;
        bool visible = false;
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
    enum AnimationID {
        Orb,
        Vortex,
        Cluster,
        Laser,
    };
    struct Component {
        AnimationID animationID;
        unsigned long layer = 1;
        float rotation = 0;
        float scale = 3;
    };

    std::string toString(Animation::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Animation

namespace Velocity {
    struct Component {
        float xVelocity = 0;
        float yVelocity = 0;
        EntityID follow = -1;
    };

    std::string toString(Velocity::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);
} // namespace Velocity

// a struct for client side use only, to get the Player Entity  via the ECS
// check if this doesnt f up the ECS synchro
namespace Player {
    struct Component {
        bool player = true;
    };

} // namespace Player

namespace Damage {
    struct Component {
        int damage = 0;
    };
} // namespace Damage

namespace Armament {
    enum ArmamentType {
        Bullet,
        Spray,
    };

    struct Component {
        Armament::ArmamentType type;
        // in milliseconds
        int interval;
        std::time_t timer = std::time(0);
        std::chrono::time_point<std::chrono::system_clock> timerc = std::chrono::system_clock::now();
        double intervalc = 0.1;
        long long ammo = -1;
    };
} // namespace Armament