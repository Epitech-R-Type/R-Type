
#include "ArmamentSystem.hpp"
#include "../../shared/ECS/ECSManager.hpp"
#include "Factory.hpp"
#include <chrono>

ArmamentSystem::ArmamentSystem(std::shared_ptr<ECSManager> ECS) {
    this->_ECS = ECS;
}

void ArmamentSystem::makeWeapon(EntityID entityID, std::shared_ptr<ECSManager> ECS) {
    Armament::Component* armament = ECS->getComponent<Armament::Component>(entityID);

    const auto now = getNow();

    std::chrono::duration<double> elapsed_seconds = now - armament->timer;

    // Convert to milliseconds
    if (!(elapsed_seconds.count() > ((double)armament->interval / 1000.0)))
        return;
    if (armament->ammo == 0)
        return;

    switch (armament->type) {
        case Armament::Type::Laser:
            Factory::Weapon::makeSingleShot(ECS, entityID, Animation::AnimationID::Laser);
            break;
        case Armament::Type::Ball:
            Factory::Weapon::makeSingleShot(ECS, entityID, Animation::AnimationID::Ball);
            break;
        case Armament::Type::Buckshot:
            Factory::Weapon::makeBuckshot(ECS, entityID, Animation::AnimationID::Ball);
            break;
    }

    if (armament->ammo > 0)
        armament->ammo -= 1;

    armament->timer = now;
}

void ArmamentSystem::apply() {
#ifndef NO_HOSTILITY
    for (auto beg = this->_ECS->begin<Armament::Component>(); beg != this->_ECS->end<Armament::Component>(); ++beg) {
        if (this->_ECS->hasComponent<Player::Component>(*beg))
            continue;
        ArmamentSystem::makeWeapon(*beg, this->_ECS);
    }
#endif
}