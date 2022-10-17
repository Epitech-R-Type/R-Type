
#pragma once
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class JanitorSystem : public System {
public:
    /**
     * System that handles deletion of entities
     * */
    JanitorSystem(ECSManager* ECS);

    /**
     * deletes all entities that match certain criteria such as:
     * - Out of Bounds
     * - Dead
     * */
    void apply();

private:
    ECSManager* _ECS;
};