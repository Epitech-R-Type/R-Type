
#pragma once

#include "../ECS/ECS.hpp"

class VelocitySystem {
public:
    VelocitySystem(Manager* ECS);

    void apply();

private:
    Manager* _ECS;
};