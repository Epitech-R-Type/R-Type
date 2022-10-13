/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/

#include "PlayerMovementSystem.hpp"
#include "../../shared/ECS/Manager.hpp"

PlayerMovementSystem::PlayerMovementSystem(Manager* ECS)
{
    this->_ECS = ECS;
}

void PlayerMovementSystem::apply()
{
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_timer;

    if (elapsed_seconds.count() > 0.1) {
        for (auto beg = this->_ECS->begin<Player::Component>(); beg != this->_ECS->end<Player::Component>(); ++beg) {
            EntityID id = *beg;
            Position::Component* position = this->_ECS->getComponent<Position::Component>(id);
            //Here is missing to check for Collision
            if (IsKeyDown(KEY_W) && position->xPos < GetScreenWidth() - 0.5) {
                this->_ECS->getComponent<Position::Component>(id)->xPos += 0.5;
            }
            if (IsKeyDown(KEY_S) && position->xPos > 0.5) {
                this->_ECS->getComponent<Position::Component>(id)->xPos -= 0.5;
            }
            if (IsKeyDown(KEY_A) && position->yPos > 0.5) {
                this->_ECS->getComponent<Position::Component>(id)->yPos -= 0.5;
            }
            if (IsKeyDown(KEY_D) && position->yPos < GetScreenHeight() - 0.5) {
                this->_ECS->getComponent<Position::Component>(id)->yPos += 0.5;
            }
        }
        this->_timer = now;
    }
}
