
#pragma once

#include "../ECS/ECS.hpp"
#include "ISystem.hpp"
#include <chrono>

class VelocitySystem : public System {
public:
    VelocitySystem(Manager* ECS);

    void apply();

private:
    Manager* _ECS;
    std::chrono::time_point<std::chrono::system_clock> _timer = std::chrono::system_clock::now();
};