#pragma once

#include "../../shared/ECS/ECS.hpp"

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
