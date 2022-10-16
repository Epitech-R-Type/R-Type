#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/systems/ISystem.hpp"

class SpriteSystem;

class ArmamentSystem : public System {
public:
    ArmamentSystem(ECSManager* ECS);

    void apply();

    void setPlayer(EntityID playerID);

    void setSpriteSystem(SpriteSystem* spriteSystem);

private:
    ECSManager* _ECS;
    EntityID _player;
    SpriteSystem* _spriteSystem;
};