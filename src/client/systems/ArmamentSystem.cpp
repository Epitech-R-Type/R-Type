
#include "ArmamentSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "SpriteSystem.hpp"
#include <chrono>

#include "../ClientGame/factories.hpp"

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
    // for (auto beg = this->_ECS->begin<Health::Component, Position::Component>(); beg != this->_ECS->end<Health::Component, Position::Component>();
    //      ++beg) {

    //     Armament::Component* armament = this->_ECS->getComponent<Armament::Component>(*beg);

    //     auto now = std::chrono::system_clock::now();

    //     std::chrono::time_point<std::chrono::system_clock> timer = armament->timer;
    //     if ((now - armament->timer).count() > armament->interval) {
    //         if (this->_ECS->hasComponent<Player::Component>(this->_player)) {
    //             makeBullet(this->_ECS, this->_spriteSystem);
    //         } else {

    //             armament->timer = now;
    //         }
    //     }
    // }
}