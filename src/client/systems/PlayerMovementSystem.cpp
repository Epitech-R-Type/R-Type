/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/

#include "PlayerMovementSystem.hpp"
#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "SpriteSystem.hpp"

PlayerMovementSystem::PlayerMovementSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void PlayerMovementSystem::setPlayer(EntityID player) {
    this->_player = player;
}

void PlayerMovementSystem::apply() {
    constexpr int moveSpeed = 10;
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - this->_timer;

    if (elapsed_seconds.count() > 0.02) {
        Position::Component* position = this->_ECS->getComponent<Position::Component>(this->_player);
        Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(this->_player);

        if (IsKeyDown(KEY_W) && position->yPos > SpriteSystem::ANIMATION_SHEET[animation->animationID].frameHeight * 3) {
            this->_ECS->getComponent<Position::Component>(this->_player)->yPos -= moveSpeed;
        }
        if (IsKeyDown(KEY_S) && position->yPos < (GetScreenHeight())) {
            this->_ECS->getComponent<Position::Component>(this->_player)->yPos += moveSpeed;
        }
        if (IsKeyDown(KEY_A) && position->xPos > SpriteSystem::ANIMATION_SHEET[animation->animationID].frameWidth * 1.5) {
            this->_ECS->getComponent<Position::Component>(this->_player)->xPos -= moveSpeed;
        }
        if (IsKeyDown(KEY_D) && position->xPos < (GetScreenWidth() - SpriteSystem::ANIMATION_SHEET[animation->animationID].frameWidth * 1.5)) {
            this->_ECS->getComponent<Position::Component>(this->_player)->xPos += moveSpeed;
        }

        this->_timer = now;
    }
}
