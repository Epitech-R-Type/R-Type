#include "JanitorSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/ECSManager.hpp"

#define INVISIBLE_BOX 2500

JanitorSystem::JanitorSystem(std::shared_ptr<ECSManager> ECS) {
    this->_ECS = ECS;
}

void JanitorSystem::apply() {
    for (auto beg = this->_ECS->begin<Health::Component>(); beg != this->_ECS->end<Health::Component>(); ++beg) {
        // Health kill off
        if (this->_ECS->getComponent<Health::Component>(*beg)->health <= 0) {
            Player::Component* player;

            // Handle player dying
            if ((player = this->_ECS->getComponent<Player::Component>(*beg)))
                this->_killedPlayers.push_back(player->uniqueID);

            this->_ECS->pushModified(*beg);
            this->_ECS->deleteEntity(*beg);
            player = nullptr;
            continue;
        }

        // Out of bounds kill off
        const Position::Component* position = this->_ECS->getComponent<Position::Component>(*beg);
        if (position->x < -400 || position->x > INVISIBLE_BOX || position->y < -400 || position->y > INVISIBLE_BOX) {
            Player::Component* player;

            // Kill player if out of bounds
            if ((player = this->_ECS->getComponent<Player::Component>(*beg)))
                this->_killedPlayers.push_back(player->uniqueID);

            this->_ECS->pushModified(*beg);
            this->_ECS->deleteEntity(*beg);
            player = nullptr;
            continue;
        }
    }
}

std::vector<int> JanitorSystem::getKilledPlayers() const {
    return this->_killedPlayers;
}

void JanitorSystem::resetKilledPlayers() {
    this->_killedPlayers.clear();
}