
#pragma once

#include "../ECS/ECS.hpp"
#include "ISystem.hpp"
class VelocitySystem : public System {
public:
    VelocitySystem(Manager* ECS);

    void apply();

private:
    Manager* _ECS;
};