/*
** EPITECH PROJECT, 2022
** Entity.hpp
** File description:
** .
*/

#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>

class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentID() noexcept {
    static ComponentID ID = getComponentID();
    return ID;
};

class Component {
  public:
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual std::string serialize() const = 0;

    virtual ~Component() = default;

    friend std::ostream& operator<<(std::ostream& out, Component const& comp) {
        return out << comp.serialize();
    }

    Entity* _entity;
};

enum componentsEnum {
    HEALTH,
    ARMOR,
    POSITION,
    COLLISION,
};

struct components : public std::map<std::string, componentsEnum> {
    components() {
        this->operator[]("Health") = componentsEnum::HEALTH;
        this->operator[]("Collision") = componentsEnum::COLLISION;
        this->operator[]("Armor") = componentsEnum::ARMOR;
        this->operator[]("Position") = componentsEnum::POSITION;
    };
    ~components() {}
};