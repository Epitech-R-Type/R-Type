
#pragma once

#include "../ECS/ECS.hpp"

class System {
public:
    virtual void apply() = 0;
};