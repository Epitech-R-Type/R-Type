/*
** EPITECH PROJECT, 2022
** Manager.hpp
** File description:
** .
*/

#pragma once

#include <vector>

#include "Entity.hpp"

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> _entities;

public:
    void draw()
    {
        for (auto &e : this->_entities)
            e->draw();
    }

    void update()
    {
        for (auto &e : this->_entities)
            e->update();
    }

    void refresh()
    {
        for (size_t i = 0; i < this->_entities.size(); i++)
            if (!(*this->_entities[i]).isActive())
            {
                this->_entities.erase(this->_entities.begin() + i);
                i--;
            }
    }

    Entity &addEntity()
    {
        Entity *e = new Entity();
        std::unique_ptr<Entity> uPtr{e};
        this->_entities.emplace_back(std::move(uPtr));
        return *e;
    }
};