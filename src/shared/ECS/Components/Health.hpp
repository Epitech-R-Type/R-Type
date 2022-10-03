/*
** EPITECH PROJECT, 2022
** Health.hpp
** File description:
** .
*/

#pragma once

#include "../Component.hpp"
#include "../Entity.hpp"
#include "Armor.hpp"

class Health : public Component {
  private:
    int _health = 0;

  public:
    void init() {
        this->_health = 0;
    }

    void setValues(int health) {
        this->_health = health;
    }

    void reduceHealth(int damage) {
        damage = this->_entity->hasComponent<Armor>() ? this->_entity->getComponent<Armor>().reduceDamage(damage) : damage;

        this->_health -= damage;

        if (this->_health <= 0) {
            this->_health = 0;
            this->_entity->destroy();
        }
    }

    int getHealth() const {
        return this->_health;
    }

    std::string serialize() const {
        return "Health(" + std::to_string(this->_health) + ")";
    }

    void update(int health) {
        this->_health = health;
    }
};
