/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/
#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "PlayerMovementSystem.hpp"
#include "SpriteSystem.hpp"

PlayerMovementSystem::PlayerMovementSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void PlayerMovementSystem::setPlayer(EntityID player) {
    this->_player = player;
}

void PlayerMovementSystem::apply() {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_timer;

    if (elapsed_seconds.count() > MOVEMENT_TIMER) {
        Position::Component* position = this->_ECS->getComponent<Position::Component>(this->_player);
        Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(this->_player);
        Velocity::Component* velocity = this->_ECS->getComponent<Velocity::Component>(this->_player);

        if (IsKeyDown(KEY_W) && position->yPos > Animation::Sheets[animation->animationID].frameHeight * 3) {
            this->_ECS->getComponent<Position::Component>(this->_player)->yPos -= velocity->yVelocity;
        }
        if (IsKeyDown(KEY_S) && position->yPos < (GetScreenHeight())) {
            this->_ECS->getComponent<Position::Component>(this->_player)->yPos += velocity->yVelocity;
        }
        if (IsKeyDown(KEY_A) && position->xPos > Animation::Sheets[animation->animationID].frameWidth * (3 / 2)) {
            this->_ECS->getComponent<Position::Component>(this->_player)->xPos -= velocity->xVelocity;
        }
        if (IsKeyDown(KEY_D) && position->xPos < (GetScreenWidth() - Animation::Sheets[animation->animationID].frameWidth * 3 / 2)) {
            this->_ECS->getComponent<Position::Component>(this->_player)->xPos += velocity->xVelocity;
        }

        this->_timer = now;
    }
}
