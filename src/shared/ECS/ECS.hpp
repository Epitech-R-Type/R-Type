#pragma once 

#include <bitset>
#include <vector>

constexpr maxBitsetSize = 16;

using ComponentID = std::size_t;
using Bitset = bitset::

ComponentID getComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> ComponentID getComponentID() noexcept
{
    static ComponentID ID = getComponentID();
    return ID;
};

class Component
{
    public:
        Entity* entity

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}
};

class Entity
{
private: 
    bool active = true;
    
    std::vector<Component> components;

public:
    void draw () {
        for (auto &c : this.components) {
            c.draw();
        }
    }

    void update () {
        for (auto &c : this.components) {
            c.update();
        }
    }

    template<typename T>
    bool hasComponent<T>() {
        
    }

    void destroy() {
        this.active = false;
    }

    bool isActive() const {
        return this.active;
    }       
}