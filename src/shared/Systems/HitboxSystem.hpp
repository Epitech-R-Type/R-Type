

#pragma once

#include "../ECS/Components.hpp"
#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class HitboxSystem : public System {
public:
    HitboxSystem(ECSManager* ECS);

    /**
     * Updates the hitbox position if the entity has moved
     * */
    Hitbox::Component* updateHitbox(EntityID entity);

    /**
     * Checks if entity collides with any other hitbox owning entity
     * */
    void checkCollision(EntityID entity);

    /**
     * checks collision and applies CollisionEffect:Component to colliding entities
     * */
    void apply();

    /**
     * Checks if 2 entities are colliding
     * */
    bool isColliding(EntityID entity1, EntityID entity2);

    /**
     * deduces Hitbox from Animation  nad Position Component
     * */
    static Hitbox::Component buildHitbox(Animation::Component* animation, Position::Component* position);

    /**
     * static version for use in other contextes
     * */
    static bool isColliding(EntityID entity1, EntityID entity2, ECSManager* ECS);

private:
    void dealDamage(EntityID defender, EntityID attacker);

    ECSManager* _ECS;
};