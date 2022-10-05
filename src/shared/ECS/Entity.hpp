/*
** EPITECH PROJECT, 2022
** Entity.hpp
** File description:
** .
*/

#pragma once

#include "Component.hpp"
#include <array>
#include <bitset>
#include <memory>
#include <string>
#include <vector>

constexpr int maxBitsetSize = 16;

using Bitset = std::bitset<maxBitsetSize>;
using ComponentArray = std::array<Component*, maxBitsetSize>;

namespace ECSError {
class InvalidComponent : std::exception {
    std::string _component;

  public:
    InvalidComponent(std::string component) {
        this->_component = component;
    }

    const char* what() const noexcept {
        std::string message = "\"" + this->_component + "\" is not a valid component.";

        return message.c_str();
    }
};

} // namespace ECSError

class Entity {
  private:
    bool _active = true;
    std::vector<std::unique_ptr<Component>> _components;
    Bitset _componentBitset;
    ComponentArray _componentArray;

  public:
    void draw() {
        for (auto& c : this->_components) {
            c->draw();
        }
    }

    void update() {
        for (auto& c : this->_components) {
            c->update();
        }
    }

    template <typename T> bool hasComponent() {
        return this->_componentBitset[getComponentID<T>()];
    }

    template <typename T> T& addComponent() {
        T* c = new T();
        c->_entity = this;
        std::unique_ptr<Component> uPtr{c};
        this->_components.emplace_back(std::move(uPtr));
        this->_componentArray[getComponentID<T>()] = c;
        this->_componentBitset[getComponentID<T>()] = true;
        c->init();
        return *c;
    }

    template <typename T> T& getComponent() const {
        auto ptr(this->_componentArray[getComponentID<T>()]);
        return *static_cast<T*>(ptr);
    }

    void destroy() {
        this->_active = false;
    }

    bool isActive() const {
        return this->_active;
    }

    template <typename T, typename... Args> void updateComponent(Args... args) {
        if (!this->hasComponent<T>())
            this->addComponent<T>();

        this->getComponent<T>().update(args...);
    }

    std::string serialize() const;

    void deserialize(std::string entity_string);

    friend std::ostream& operator<<(std::ostream& out, Entity const& entity) {
        return out << entity.serialize();
    }
};
