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

PlayerMovementSystem::PlayerMovementSystem(ECSManager* ECS) {
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

        if (IsKeyDown(KEY_A) && position->x > 0) {
            this->_ECS->getComponent<Position::Component>(this->_player)->x -= velocity->x;
        }
        if (IsKeyDown(KEY_D) && position->x < (GetScreenWidth() - (Animation::Sheets[animation->animationID].frameWidth * animation->scale))) {
            this->_ECS->getComponent<Position::Component>(this->_player)->x += velocity->x;
        }
        if (IsKeyDown(KEY_W) && position->y > 0) {
            this->_ECS->getComponent<Position::Component>(this->_player)->y -= velocity->y;
        }
        if (IsKeyDown(KEY_S) && position->y < (GetScreenHeight() - (Animation::Sheets[animation->animationID].frameHeight * animation->scale))) {
            this->_ECS->getComponent<Position::Component>(this->_player)->y += velocity->y;
        }

        this->_timer = now;
    }
}
