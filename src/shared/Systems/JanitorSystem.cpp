#include "JanitorSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"

#define INVISIBLE_BOX 2500

JanitorSystem::JanitorSystem(std::shared_ptr<ECSManager> ECS) {
    this->_ECS = ECS;
}

void JanitorSystem::apply() {
    for (auto beg = this->_ECS->begin<Health::Component>(); beg != this->_ECS->end<Health::Component>(); ++beg) {
        if (this->_ECS->getComponent<Health::Component>(*beg)->health <= 0) {
            this->_ECS->deleteEntity(*beg);
            continue;
        }

        const Position::Component* position = this->_ECS->getComponent<Position::Component>(*beg);

        if (position->x < -100 || position->x > INVISIBLE_BOX || position->y < -100 || position->y > INVISIBLE_BOX) {
            this->_ECS->deleteEntity(*beg);
            continue;
        }
    }
}