#pragma once

#include "ECS.hpp"

class Health;

class Armor;
class Position;

class Position : public Component
{
private:
    int xpos;
    int ypos;

public:
    // PositionComponent();
    int x() { return xpos; }

    int y() { return ypos; }

    void init() override
    {
        xpos = 3;
        ypos = 3;
    }

    void setPos(int x, int y)
    {
        xpos = x;
        ypos = y;
    }
};

class Armor : public Component
{
private:
    int _armor = 0;

public:
    void init()
    {
        this->_armor = 0;
    }

    void setValues(int armor)
    {

        this->_armor = armor;
    }

    int reduceDamage(int damage)
    {
        return damage - this->_armor >= 0 ? damage - this->_armor : 0;
    }
};
class Health : public Component
{
private:
    int _health = 0;

public:
    void init()
    {
        this->_health = 0;
    }

    void setValues(int health)
    {

        this->_health = health;
    }

    void reduceHealth(int damage)
    {
        damage = this->entity->hasComponent<Armor>() ? this->entity->getComponent<Armor>().reduceDamage(damage) : damage;

        this->_health -= damage;

        if (this->_health <= 0)
        {
            this->_health = 0;
            this->entity->destroy();
        }
    }

    int getHealth()
    {
        return this->_health;
    }
};

class CollisionEffect : public Component
{
private:
    int _damage = 0;
    int _healing = 0;

public:
    void init()
    {
        this->_damage = 0;
        this->_healing = 0;
    }

    void setEffect(int damage, int healing)
    {

        this->_damage = damage;
        this->_healing = healing;
    }

    void collide(Entity &entity)
    {
        entity.getComponent<Health>().reduceHealth(this->_damage);
        entity.getComponent<CollisionEffect>().reverseCollide(this->entity);
    }

    void reverseCollide(Entity *entity)
    {
        entity->getComponent<Health>().reduceHealth(this->_damage);
    }
};
