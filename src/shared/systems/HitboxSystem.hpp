

#pragma once

#include "../ECS/ECS.hpp";
#include "ISystem.hpp"

class HitboxSystem : public System {
    HitboxSystem(Manager* ECS);

    void apply();

    void hitboxFromAnimation(EntityID entity);

private:
    Manager* _ECS;
};