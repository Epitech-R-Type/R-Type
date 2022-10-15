
#pragma once
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class JanitorSystem : public System {
public:
    JanitorSystem(Manager* ECS);
    void apply();

private:
    Manager* _ECS;
};