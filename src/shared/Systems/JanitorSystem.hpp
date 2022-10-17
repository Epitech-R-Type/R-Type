
#pragma once
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class JanitorSystem : public System {
public:
    /**
     * System that handles deletion of entities
     * */
    JanitorSystem(std::shared_ptr<ECSManager> ECS);

    /**
     * deletes all entities that match certain criteria such as:
     * - Out of Bounds
     * - Dead
     * */
    void apply();

private:
    std::shared_ptr<ECSManager> _ECS;
};