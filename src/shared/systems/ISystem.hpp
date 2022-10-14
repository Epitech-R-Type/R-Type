
#pragma once

#define MOVEMENT_TIMER 0.02

#include "../ECS/ECS.hpp"

class System {
public:
    virtual void apply() = 0;
};