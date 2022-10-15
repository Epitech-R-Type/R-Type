#include "JanitorSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"

#define INVISIBLE_BOX 3000

JanitorSystem::JanitorSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void JanitorSystem::apply() {
    for (auto beg = this->_ECS->begin<Health::Component>(); beg != this->_ECS->end<Health::Component>(); ++beg) {
        if (this->_ECS->getComponent<Health::Component>(*beg)->health <= 0) {
            this->_ECS->deleteEntity(*beg);
            continue;
        }

        const Position::Component* position = this->_ECS->getComponent<Position::Component>(*beg);

        if (position->xPos < -INVISIBLE_BOX || position->xPos > INVISIBLE_BOX || position->yPos < -INVISIBLE_BOX || position->yPos > INVISIBLE_BOX) {
            this->_ECS->deleteEntity(*beg);
            continue;
        }
    }
}