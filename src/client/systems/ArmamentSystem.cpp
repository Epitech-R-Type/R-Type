
#include "ArmamentSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../ClientGame/factories.hpp"
#include "SpriteSystem.hpp"
#include <chrono>

ArmamentSystem::ArmamentSystem(ECSManager* ECS) {
    this->_ECS = ECS;
}

void ArmamentSystem::setPlayer(EntityID player) {
    this->_player = player;
}
void ArmamentSystem::setSpriteSystem(SpriteSystem* spriteSystem) {
    this->_spriteSystem = spriteSystem;
}

void ArmamentSystem::apply() {
#ifndef NO_HOSTILITY
    for (auto beg = this->_ECS->begin<Armament::Component>(); beg != this->_ECS->end<Armament::Component>(); ++beg) {
        if (this->_ECS->hasComponent<Player::Component>(*beg) && !IsKeyDown(KEY_SPACE))
            continue;

        Armament::Component* armament = this->_ECS->getComponent<Armament::Component>(*beg);

        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - armament->timer;

        // Convert to milliseconds
        if (elapsed_seconds.count() > ((double)armament->interval / 1000.0)) {

            if (armament->ammo != 0)
                makeBullet(this->_ECS, this->_spriteSystem, *beg);
            if (armament->ammo > 0)
                armament->ammo -= 1;

            armament->timer = now;
        }
    }
#endif
}