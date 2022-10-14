
#include "ArmamentSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../ClientGame/factories.hpp"
#include "SpriteSystem.hpp"
#include <chrono>

ArmamentSystem::ArmamentSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void ArmamentSystem::setPlayer(EntityID player) {
    this->_player = player;
}
void ArmamentSystem::setSpriteSystem(SpriteSystem* spriteSystem) {
    this->_spriteSystem = spriteSystem;
}

void ArmamentSystem::apply() {
    for (auto beg = this->_ECS->begin<Armament::Component>(); beg != this->_ECS->end<Armament::Component>(); ++beg) {

        Armament::Component* armament = this->_ECS->getComponent<Armament::Component>(*beg);

        std::time_t now = std::time(0);

        if ((now - armament->timer) > armament->interval) {
            if (this->_ECS->hasComponent<Player::Component>(this->_player)) {
                makeBullet(this->_ECS, this->_spriteSystem);
            } else {
            }
            armament->timer = now;
        }
    }
}