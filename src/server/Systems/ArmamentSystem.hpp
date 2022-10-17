#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/Systems/ISystem.hpp"

class SpriteSystem;

class ArmamentSystem : public System {
public:
    ArmamentSystem(ECSManager* ECS);

    void apply();

private:
    ECSManager* _ECS;
};