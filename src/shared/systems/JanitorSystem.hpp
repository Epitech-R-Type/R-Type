
#pragma once
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class JanitorSystem : public System {
public:
    JanitorSystem(ECSManager* ECS);
    void apply();

private:
    ECSManager* _ECS;
};