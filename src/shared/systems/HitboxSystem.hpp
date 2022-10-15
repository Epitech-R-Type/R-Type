

#pragma once

#include "../ECS/ECS.hpp";
#include "ISystem.hpp"

class HitboxSystem : public System {
    HitboxSystem(Manager* ECS);

    void apply();

    void hitboxFromAnimation(EntityID entity);

    EntityID checkCollision(EntityID entity);

    bool isColliding(EntityID entity1, EntityID entity2);

private:
    Manager* _ECS;
};