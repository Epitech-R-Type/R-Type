/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

#include "../Entity.hpp"

// Armor component
// Armor is used to modify behaviour when mutating health component
class Armor : public Component {
  private:
    int _armor = 0;

  public:
    void init() {
        this->_armor = 0;
    }

    void setValues(int armor) {

        this->_armor = armor;
    }

    int reduceDamage(int damage) {
        return damage - this->_armor >= 0 ? damage - this->_armor : 0;
    }

    std::string serialize() const {
        return "Armor(" + std::to_string(this->_armor) + ")";
    }

    void update(int armor) {
        this->_armor = armor;
    }
};