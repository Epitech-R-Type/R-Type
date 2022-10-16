

#pragma once

#include "../ECS/Components.hpp"
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class HitboxSystem : public System {
public:
    HitboxSystem(ECSManager* ECS);

    void apply();

    static Hitbox::Component buildHitbox(Animation::Component* animation, Position::Component* position);

    Hitbox::Component* updateHitbox(EntityID entity);

    void checkCollision(EntityID entity);

    bool isColliding(EntityID entity1, EntityID entity2);

    static bool isColliding(EntityID entity1, EntityID entity2, ECSManager* ECS);

    static Point rotate(Point point, Point origin, double rad);

    static double toRadians(double degree);

private:
    void dealDamage(EntityID defender, EntityID attacker);

    ECSManager* _ECS;
};