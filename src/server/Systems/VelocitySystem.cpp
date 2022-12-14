
#include "VelocitySystem.hpp"
#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECSManager.hpp"

VelocitySystem::VelocitySystem(std::shared_ptr<ECSManager> ECS) {
    this->_ECS = ECS;
}

void VelocitySystem::apply() {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_timer;
    if (elapsed_seconds.count() > MOVEMENT_TIMER) {
        this->_ECS->setExcluded<Player::Component>();
        for (auto beg = this->_ECS->begin<Position::Component, Velocity::Component>();
             beg != this->_ECS->end<Position::Component, Velocity::Component>(); ++beg) {
            EntityID id = *beg;

            if (this->_ECS->hasComponent<Player::Component>(id))
                continue;

            Velocity::Component* velocity = this->_ECS->getComponent<Velocity::Component>(id);
            Position::Component* position = this->_ECS->getComponent<Position::Component>(id);

            if (velocity->follow >= 0) {
                const Position::Component* trackedEntityPosition = this->_ECS->getComponent<Position::Component>(velocity->follow);

                position->x = trackedEntityPosition->x;
                position->y = trackedEntityPosition->y;
            } else {
                position->x += velocity->x;
                position->y += velocity->y;
            }

            this->_ECS->pushModified(*beg);
        }
        this->_timer = now;
    }
    this->_ECS->resetExcluded();
};