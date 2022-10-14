#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/systems/ISystem.hpp"

class HealthSystem : public System {
public:
    HealthSystem(Manager* ECS);

    void apply();

    void setPlayer(EntityID playerID);

    void drawPlayerHP();

private:
    Manager* _ECS;
    EntityID _player;
};