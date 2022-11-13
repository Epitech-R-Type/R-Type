/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "../Networking/ProtocolUtils.hpp"
#include "../Utilities/Utilities.hpp"
#include "ECS.hpp"
#include "ECSManager.hpp"
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

// BUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUFER Related

template <class T>
ByteBuf initBuffer(ComponentType id) {
    ByteBuf buffer;
    buffer.resize(sizeof(std::int8_t) + sizeof(std::uint64_t) + sizeof(T));
    buffer[0] = id;
    const std::uint64_t size = sizeof(T);
    memcpy(&buffer[1], (std::int8_t*)&size, sizeof(size));
    return buffer;
}

ComponentType getComponentType(ByteBuf buffer);

std::uint64_t getComponentSize(ByteBuf buffer);

template <class T>
T getComponentData(ByteBuf buffer) {
    return *(T*)&(buffer[1 + sizeof(getComponentSize(buffer))]);
}

template <class T>
void insertComponentData(ByteBuf& buffer, T data) {
    memcpy(&buffer[9], (std::int8_t*)&data, sizeof(T));
}

// ENDOF

template <typename T, typename M>
ByteBuf toBuffer(T component, ComponentType ID) {
    ByteBuf buffer = initBuffer<T>(ID);

    M mask = *(M*)(&component);

    insertComponentData<M>(buffer, mask);

    return buffer;
}

template <typename T>
void applyUpdate(ByteBuf buffer, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    T* component;

    if (!manager->hasComponent<T>(entityID))
        component = manager->addComp<T>(entityID, {});
    else
        component = manager->getComponent<T>(entityID);

    *component = getComponentData<T>(buffer);
}

namespace Armor {
    struct Component {
        int armor = 0;
    };

    struct Mask {
        int armor = 0;
    };

} // namespace Armor

namespace Health {
    struct Component {
        int health = 0;
        int maxHealth = 0;
        bool visible = false;
    };

    struct Mask {
        int health = 0;
        int maxHealth = 0;
        bool visible = false;
    };

} // namespace Health

namespace Position {
    struct Component {
        float x = 0;
        float y = 0;
    };

    struct Mask {
        float x = 0;
        float y = 0;
    };

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
        Background,
        Death,
    };
    struct Component {
        AnimationID animationID;
        unsigned long layer = 1;
        double rotation = 0;
        double scale = 3;
        int index = 0;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    struct Mask {
        AnimationID animationID;
        unsigned long layer = 1;
        double rotation = 0;
        double scale = 3;
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

        {Animation::AnimationID::SpaceshipLightblue, {"resources/r-typesheet42.png", 1, 3, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipPink, {"resources/r-typesheet42.png", 1, 20, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipGreen, {"resources/r-typesheet42.png", 1, 37, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipRed, {"resources/r-typesheet42.png", 1, 54, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipDarkblue, {"resources/r-typesheet42.png", 1, 71, 32, 16, 5, 1, 0, 0, 1, 0.2}},
        {Animation::AnimationID::SpaceshipRGB, {"resources/r-typesheet42.png", 1, 3, 32, 16, 1, 5, 0, 1, 0, 0.2}},

        {Animation::AnimationID::Background, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
        {Animation::AnimationID::Death, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
    };

} // namespace Animation

namespace Velocity {
    struct Component {
        float x = 0;
        float y = 0;
        EntityID follow = -1;
        double tickrate = 0.02;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    struct Mask {
        float x = 0;
        float y = 0;
        EntityID follow = -1;
    };
} // namespace Velocity

namespace Player {
    struct Component {
        int score;
        int uniqueID;
    };

    struct Mask {
        int score;
    };

} // namespace Player

namespace Damage {
    struct Component {
        int damage = 0;
    };

    struct Mask {
        int damage = 0;
    };

} // namespace Damage

namespace Armament {
    enum Type {
        Laser,
        Buckshot,
    };

    struct Component {
        Armament::Type type;
        double interval = 0.1; // in milliseconds
        long long ammo = -1;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    struct Mask {
        Armament::Type type;
        double interval = 0.1; // in milliseconds
        long long ammo = -1;
    };

} // namespace Armament

namespace Hitbox {
    struct Component {
        Point topLeft;
        Point topRight;
        Point botLeft;
        Point botRight;
    };

    struct Mask {
        Point topLeft;
        Point topRight;
        Point botLeft;
        Point botRight;
    };

} // namespace Hitbox

namespace Team {
    enum Group {
        Ally,
        Enemy,
    };

    struct Component {
        Group team;
    };

    struct Mask {
        Group team;
    };

} // namespace Team

namespace ImmunityFrame {
    struct Component {
        double duration = 0;
        std::chrono::time_point<std::chrono::system_clock> timer;
    };

    struct Mask {
        double duration = 0;
    };

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

    struct Mask {
        SFXID ID;
    };

} // namespace SoundCreation

namespace SoundDestruction {
    struct Component {
        SFXID ID;
    };

    struct Mask {
        SFXID ID;
    };

} // namespace SoundDestruction

namespace SoundDamage {
    struct Component {
        SFXID ID;
    };

    struct Mask {
        SFXID ID;
    };

} // namespace SoundDamage

// std::string toString(Armor::Component component);
// std::string toString(Health::Component component);
// std::string toString(Position::Component component);
// std::string toString(Animation::Component component);
// std::string toString(Velocity::Component component);
// std::string toString(Player::Component component);
// std::string toString(Damage::Component component);
// std::string toString(Armament::Component component);
// std::string toString(Hitbox::Component component);
// std::string toString(Team::Component component);
// std::string toString(ImmunityFrame::Component component);
// std::string toString(CollisionEffect::Component component);
// std::string toString(SoundCreation::Component component);
// std::string toString(SoundDestruction::Component component);
// std::string toString(SoundDamage::Component component);
