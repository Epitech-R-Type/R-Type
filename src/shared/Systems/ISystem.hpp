
#pragma once

#define MOVEMENT_TIMER 0.02

#include "../ECS/ECS.hpp"
#include <memory>

class System {
public:
    virtual void apply() = 0;

private:
    std::shared_ptr<ECSManager> _ECS;
};