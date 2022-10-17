#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/Systems/ISystem.hpp"

class HealthSystem : public System {
public:
    HealthSystem(std::shared_ptr<ECSManager> ECS);

    void apply();

    void setPlayer(EntityID playerID);

    void drawPlayerHP();

private:
    std::shared_ptr<ECSManager> _ECS;
    EntityID _player;
};