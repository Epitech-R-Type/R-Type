/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/
#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECSManager.hpp"
#include "PlayerMovementSystem.hpp"
#include "raylib.h"

PlayerMovementSystem::PlayerMovementSystem(std::shared_ptr<ClientGameProtocol> protocol) {
    this->_protocol = protocol;
}

void PlayerMovementSystem::apply() {

    if (IsKeyDown(KEY_A))
        this->_protocol->sendActMove(Move::LEFT);
    if (IsKeyDown(KEY_D))
        this->_protocol->sendActMove(Move::RIGHT);
    if (IsKeyDown(KEY_W))
        this->_protocol->sendActMove(Move::UP);
    if (IsKeyDown(KEY_S))
        this->_protocol->sendActMove(Move::DOWN);
    if (IsKeyDown(KEY_SPACE))
        this->_protocol->sendActFire();
}
