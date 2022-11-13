#pragma once

#include "../../shared/ECS/ECS.hpp"
#include "../Game.hpp"
#include <memory>

void dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS);

namespace Factory {
    namespace Ally {
        EntityID makePlayer(std::shared_ptr<ECSManager> ECS, int uniqueID);
    }
    namespace Enemy {
        EntityID makeEndboss(std::shared_ptr<ECSManager> ECS, BossStats stats);

        EntityID makeEnemy(std::shared_ptr<ECSManager> ECS, EnemyStats stats);
    } // namespace Enemy
    namespace Weapon {
        void makeSingleShot(std::shared_ptr<ECSManager> ECS, EntityID source, Animation::AnimationID anim);

        void makeBuckshot(std::shared_ptr<ECSManager> ECS, EntityID source, Animation::AnimationID anim);
    } // namespace Weapon

    namespace Misc {
        void makeBackground(std::shared_ptr<ECSManager> ECS, Animation::AnimationID bgID);
    }
} // namespace Factory
