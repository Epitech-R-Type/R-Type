
#include "VelocitySystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"

VelocitySystem::VelocitySystem(Manager* ECS) {
    this->_ECS = ECS;
}

void VelocitySystem::apply() {
    for (auto beg = this->_ECS->begin<Position::Component, Velocity::Component>(); beg != this->_ECS->end<Position::Component, Velocity::Component>();
         ++beg) {
        EntityID id = *beg;
        Velocity::Component* velocity = this->_ECS->getComponent<Velocity::Component>(id);
        Position::Component* position = this->_ECS->getComponent<Position::Component>(id);

        if (!velocity || !position)
            continue;

        position->xPos += velocity->xVelocity;
        position->yPos += velocity->yVelocity;
    }
};