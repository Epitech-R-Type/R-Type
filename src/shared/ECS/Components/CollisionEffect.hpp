/*
** EPITECH PROJECT, 2022
** CollisionEffect.hpp
** File description:
** .
*/

#pragma once

#include "../Component.hpp"
#include "../Entity.hpp"
#include "Health.hpp"

class CollisionEffect : public Component {
  private:
    int _damage = 0;
    int _healing = 0;

  public:
    void init() {
        this->_damage = 0;
        this->_healing = 0;
    }

    void setEffect(int damage, int healing) {

        this->_damage = damage;
        this->_healing = healing;
    }

    void collide(Entity& entity) {
        entity.getComponent<Health>().reduceHealth(this->_damage);
        entity.getComponent<CollisionEffect>().reverseCollide(this->_entity);
    }

    void reverseCollide(Entity* entity) {
        entity->getComponent<Health>().reduceHealth(this->_damage);
    }

    std::string serialize() const {
        return "CollisionEffect(" + std::to_string(this->_damage) + "," + std::to_string(this->_healing) + ")";
    }

    void update(int damage, int healing) {
        this->_damage = damage;
        this->_healing = healing;
    }
};
