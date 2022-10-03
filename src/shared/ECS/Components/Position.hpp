/*
** EPITECH PROJECT, 2022
** Position.hpp
** File description:
** .
*/

#pragma once

#include "../Component.hpp"
#include "../Entity.hpp"

// Postion component
// Reflects position of entity

class Position : public Component {
  private:
    int _xpos;
    int _ypos;

  public:
    int x() {
        return this->_xpos;
    }
    int y() {
        return this->_ypos;
    }

    void init() override {
        this->_xpos = 3;
        this->_ypos = 3;
    }

    void setPos(int x, int y) {
        this->_xpos = x;
        this->_ypos = y;
    }

    std::string serialize() const {
        return "Position(" + std::to_string(this->_xpos) + "," + std::to_string(this->_ypos) + ")";
    }

    void update(int x, int y) {
        this->_xpos = x;
        this->_ypos = y;
    }
};