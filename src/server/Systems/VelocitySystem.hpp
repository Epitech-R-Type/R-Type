
#pragma once

#include "../../shared/ECS/ECS.hpp"
#include "../../shared/Systems/ISystem.hpp"
#include <chrono>

class VelocitySystem : public System {
public:
    /**
     * System that updates entity positions using their x and y velocity
     * or sets their position to the followed entity's position
     * */
    VelocitySystem(std::shared_ptr<ECSManager> ECS);

    void apply();

private:
    std::shared_ptr<ECSManager> _ECS;

    std::chrono::time_point<std::chrono::system_clock> _timer = std::chrono::system_clock::now();
};