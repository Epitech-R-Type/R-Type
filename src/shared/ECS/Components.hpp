/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "ECS.hpp"
#include <ctime>
#include <map>
#include <sstream>
#include <vector>

struct Point {
    double x;
    double y;
};

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
        Lost,
    };
    struct Component {
        AnimationID animationID;
        unsigned long layer = 1;
        float rotation = 0;
        float scale = 3;
        int index = 0;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    struct Sheet {
        std::string path;
        float startX;
        float startY;
        float frameWidth;
        float frameHeight;
        int animWidth;
        int animHeight;
        int separationX;
        int separationY;
        // adds the same frames in reverse to the animation
        bool reverse;
    };

    static std::map<Animation::AnimationID, Animation::Sheet> Sheets = {
        {Animation::AnimationID::Orb, {"resources/r-typesheet3.png", 1, 1, 16, 16, 12, 1, 1, 0, 0}},
        {Animation::AnimationID::Vortex, {"resources/r-typesheet30a.png", 1, 3, 32, 32, 3, 1, 2, 0, 0}},
        {Animation::AnimationID::Cluster, {"resources/r-typesheet32.png", 0, 0, 259, 142, 2, 3, 1, 1, 1}},
        {Animation::AnimationID::Laser, {"resources/r-typesheet43.png", 1, 41, 48, 4, 8, 1, 2, 0, 0}},
        {Animation::AnimationID::Lost, {"resources/lost.png", 0, 0, 639, 513, 8, 1, 1, 0, 0}},
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
        int score;
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
        double interval = 0.1;
        long long ammo = -1;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };
} // namespace Armament

namespace Hitbox {
    struct Component {
        Point topLeft;
        Point topRight;
        Point botLeft;
        Point botRight;
    };

    std::string toString(Hitbox::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager);

    inline float getWidth(const Animation::Component* animation) {
        return (Animation::Sheets[animation->animationID].frameWidth * animation->scale);
    }

    inline float getHeight(const Animation::Component* animation) {
        return (Animation::Sheets[animation->animationID].frameHeight * animation->scale);
    }
} // namespace Hitbox

namespace Team {
    enum Component {
        Ally,
        Enemy,
    };
} // namespace Team
