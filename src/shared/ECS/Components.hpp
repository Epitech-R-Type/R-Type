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
    SOUND_CREATION,
    SOUND_DESTRUCTION,
    SOUND_DAMAGE
};

namespace Armor {
    struct Component {
        int armor = 0;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Armor

namespace Health {
    struct Component {
        int health = 0;
        int maxHealth = 0;
        bool visible = false;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Health

namespace Position {
    struct Component {
        float x = 0;
        float y = 0;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Position

namespace Animation {
    enum AnimationID {
        Orb,
        Vortex,
        Cluster,
        Laser,
        Lost,
        SpaceshipLightblue,
        SpaceshipPink,
        SpaceshipGreen,
        SpaceshipRed,
        SpaceshipDarkblue,
        SpaceshipRGB,
        StarBg,
        Death,
        LegRobot,
        Transformer,
        Ball,
        CityBg,
        MenuBackground
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
        {Animation::AnimationID::Lost, {"resources/lost.png", 0, 0, 639, 513, 8, 1, 1, 0, 0}},
        {Animation::AnimationID::LegRobot, {"resources/r-typesheet10.png", 1, 1, 30, 23, 6, 1, 3, 0, 0}},
        {Animation::AnimationID::Transformer, {"resources/r-typesheet14.png", 1, 1, 47, 48, 5, 1, 3, 0, 0}},

        {Animation::AnimationID::Ball, {"resources/r-typesheet14.png", 182, 139, 13, 12, 8, 1, 3, 0, 0}},
        {Animation::AnimationID::Laser, {"resources/r-typesheet43.png", 1, 41, 48, 4, 8, 1, 2, 0, 0}},

        {Animation::AnimationID::SpaceshipLightblue, {"resources/r-typesheet42.png", 1, 3, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipPink, {"resources/r-typesheet42.png", 1, 20, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipGreen, {"resources/r-typesheet42.png", 1, 37, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipRed, {"resources/r-typesheet42.png", 1, 54, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipDarkblue, {"resources/r-typesheet42.png", 1, 71, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipRGB, {"resources/r-typesheet42.png", 1, 3, 32, 16, 1, 5, 0, 1, 0, 0.2}},

        {Animation::AnimationID::StarBg, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
        {Animation::AnimationID::CityBg, {"resources/citySunnset.png", 0, 0, 800, 450, 1, 1, 0, 0, 0, 0, 1}},

        {Animation::AnimationID::Death, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
        {Animation::AnimationID::MenuBackground, {"resources/menu-background.png", 0, 0, 3200, 1600, 1, 1, 0, 0, 0, 0, 1}},

    };

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

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Velocity

// a struct for client side use only, to get the Player Entity  via the ECS
// check if this doesnt f up the ECS synchro
namespace Player {
    struct Component {
        int score;
        int uniqueID;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Player

namespace Damage {
    struct Component {
        int damage = 0;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Damage

namespace Armament {
    enum Type {
        Laser,
        Buckshot,
        Ball,
    };

    struct Component {
        Armament::Type type;
        // in milliseconds
        double interval = 0.1;
        long long ammo = -1;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Armament

namespace Hitbox {
    struct Component {
        Point topLeft;
        Point topRight;
        Point botLeft;
        Point botRight;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace Hitbox

namespace Team {
    enum Component {
        Ally,
        Enemy,
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

} // namespace Team

namespace ImmunityFrame {
    struct Component {
        double duration = 0;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

} // namespace ImmunityFrame

namespace CollisionEffect {
    typedef void (*Component)(EntityID defender, EntityID attacker, std::shared_ptr<ECSManager> ECS);

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);

    void dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS);
    // should not need serialization or update since logic happens serverside
    // if we do need to just use a map and enum
} // namespace CollisionEffect

namespace SoundCreation {
    struct Component {
        SFXID ID;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace SoundCreation

namespace SoundDestruction {
    struct Component {
        SFXID ID;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace SoundDestruction

namespace SoundDamage {
    struct Component {
        SFXID ID;
    };

    void applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager);
} // namespace SoundDamage
std::string toString(Armor::Component component);
std::string toString(Health::Component component);
std::string toString(Position::Component component);
std::string toString(Animation::Component component);
std::string toString(Velocity::Component component);
std::string toString(Player::Component component);
std::string toString(Damage::Component component);
std::string toString(Armament::Component component);
std::string toString(Hitbox::Component component);
std::string toString(Team::Component component);
std::string toString(ImmunityFrame::Component component);
std::string toString(CollisionEffect::Component component);
std::string toString(SoundCreation::Component component);
std::string toString(SoundDestruction::Component component);
std::string toString(SoundDamage::Component component);
