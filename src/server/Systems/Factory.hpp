#pragma once

#include "../../shared/ECS/ECS.hpp"

void dealDamage(EntityID attacker, EntityID defender, ECSManager* ECS);

namespace Factory {
    namespace Ally {
        EntityID makePlayer(ECSManager* ECS);
    }
    namespace Enemy {
        void makeEndboss(ECSManager* ECS);

        void makeEnemy(ECSManager* ECS);
    } // namespace Enemy
    namespace Weapon {
        void makeLaser(ECSManager* ECS, EntityID source);

        void makeBuckshot(ECSManager* ECS, EntityID source);
    } // namespace Weapon
} // namespace Factory
