#pragma once

#include "../../shared/ECS/ECS.hpp"
#include <memory>

void dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS);

namespace Factory {
    namespace Ally {
        EntityID makePlayer(std::shared_ptr<ECSManager> ECS, int uniqueID);
    }
    namespace Enemy {
        EntityID makeEndboss(std::shared_ptr<ECSManager> ECS);

        EntityID makeEnemy(std::shared_ptr<ECSManager> ECS);
    } // namespace Enemy
    namespace Weapon {
        void makeLaser(std::shared_ptr<ECSManager> ECS, EntityID source);

        void makeBuckshot(std::shared_ptr<ECSManager> ECS, EntityID source);
    } // namespace Weapon

    namespace Misc {
        void makeBackground(std::shared_ptr<ECSManager> ECS);
    }
} // namespace Factory
