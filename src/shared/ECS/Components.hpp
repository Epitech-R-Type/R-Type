/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "../Utilities/Utilities.hpp"
#include "ECS.hpp"
#include <ctime>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

enum ComponentType {
    ARMOR,
    HEALTH,
    POSITION,
    ANIMATION,
    VELOCITY,
    PLAYER,
    DAMAGE,
    ARMAMENT,
    HITBOX,
    TEAM,
    IMMUNITY_FRAME,
    COLLISIONEFFECT,
};

namespace Armor {
    struct Component {
        int armor = 0;
    };

    std::string toString(Armor::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Armor

namespace Health {
    struct Component {
        int health = 0;
        int maxHealth = 0;
        bool visible = false;
    };

    std::string toString(Health::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Health

namespace Position {
    struct Component {
        float x = 0;
        float y = 0;
    };

    std::string toString(Position::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Position

namespace Animation {
    enum AnimationID {
        Orb,
        Vortex,
        Cluster,
        Laser,
        Lost,
        Spaceship,
        Background,
    };
    struct Component {
        AnimationID animationID;
        unsigned long layer = 1;
        double rotation = 0;
        double scale = 3;
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
        double interval = 0.1;
        bool tile = false;
    };

    static std::map<Animation::AnimationID, Animation::Sheet> Sheets = {
        {Animation::AnimationID::Orb, {"resources/r-typesheet3.png", 1, 1, 16, 16, 12, 1, 1, 0, 0}},
        {Animation::AnimationID::Vortex, {"resources/r-typesheet30a.png", 1, 3, 32, 32, 3, 1, 2, 0, 0}},
        {Animation::AnimationID::Cluster, {"resources/r-typesheet32.png", 0, 0, 259, 142, 2, 3, 1, 1, 1}},
        {Animation::AnimationID::Laser, {"resources/r-typesheet43.png", 1, 41, 48, 4, 8, 1, 2, 0, 0}},
        {Animation::AnimationID::Lost, {"resources/lost.png", 0, 0, 639, 513, 8, 1, 1, 0, 0}},
        {Animation::AnimationID::Spaceship, {"resources/r-typesheet42.png", 1, 3, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::Background, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
    };

    std::string toString(Animation::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Animation

namespace Velocity {
    struct Component {
        float x = 0;
        float y = 0;
        EntityID follow = -1;
        double tickrate = 0.02;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    std::string toString(Velocity::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Velocity

// a struct for client side use only, to get the Player Entity  via the ECS
// check if this doesnt f up the ECS synchro
namespace Player {
    struct Component {
        int score;
        int uniqueID;
    };

    std::string toString(Player::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Player

namespace Damage {
    struct Component {
        int damage = 0;
    };

    std::string toString(Damage::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Damage

namespace Armament {
    enum Type {
        Laser,
        Buckshot,
    };

    struct Component {
        Armament::Type type;
        // in milliseconds
        double interval = 0.1;
        long long ammo = -1;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    std::string toString(Armament::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Armament

namespace Hitbox {
    struct Component {
        Point topLeft;
        Point topRight;
        Point botLeft;
        Point botRight;
    };

    std::string toString(Hitbox::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Hitbox

namespace Team {
    enum Component {
        Ally,
        Enemy,
    };

    std::string toString(Team::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

} // namespace Team

namespace ImmunityFrame {
    struct Component {
        double duration = 0;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    std::string toString(ImmunityFrame::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

} // namespace ImmunityFrame

namespace CollisionEffect {
    typedef void (*Component)(EntityID defender, EntityID attacker, std::shared_ptr<ECSManager> ECS);

    std::string toString(CollisionEffect::Component component);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

    void dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS);
    // should not need serialization or update since logic happens serverside
    // if we do need to just use a map and enum
} // namespace CollisionEffect