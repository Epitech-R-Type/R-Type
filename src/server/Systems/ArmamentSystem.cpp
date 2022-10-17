
#include "ArmamentSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "Factory.hpp"
#include <chrono>

ArmamentSystem::ArmamentSystem(ECSManager* ECS) {
    this->_ECS = ECS;
}

void ArmamentSystem::apply() {
#ifndef NO_HOSTILITY
    for (auto beg = this->_ECS->begin<Armament::Component>(); beg != this->_ECS->end<Armament::Component>(); ++beg) {
        Armament::Component* armament = this->_ECS->getComponent<Armament::Component>(*beg);

        const auto now = getNow();

        std::chrono::duration<double> elapsed_seconds = now - armament->timer;

        // Convert to milliseconds
        if (!(elapsed_seconds.count() > ((double)armament->interval / 1000.0)))
            continue;
        if (armament->ammo == 0)
            continue;

        switch (armament->type) {
            case Armament::Type::Laser:
                Factory::Weapon::makeLaser(this->_ECS, *beg);
                break;
            case Armament::Type::Buckshot:
                Factory::Weapon::makeBuckshot(this->_ECS, *beg);
                break;
        }

        if (armament->ammo > 0)
            armament->ammo -= 1;

        armament->timer = now;
    }
#endif
}