/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/
#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/ECSManager.hpp"
#include "../Protocols/ClientGameProtocol.hpp"
#include "PlayerMovementSystem.hpp"
#include "raylib.h"

PlayerMovementSystem::PlayerMovementSystem(std::shared_ptr<ClientGameProtocol> protocol) {
    this->_protocol = protocol;
}

void PlayerMovementSystem::apply() {
    int directions = 0;
    if (IsKeyDown(KEY_A))
        directions += Move::LEFT;
    if (IsKeyDown(KEY_D))
        directions += Move::RIGHT;
    if (IsKeyDown(KEY_W))
        directions += Move::UP;
    if (IsKeyDown(KEY_S))
        directions += Move::DOWN;
    if (directions != 0)
        this->_protocol->sendActMove(std::to_string(directions));
    if (IsKeyDown(KEY_SPACE))
        this->_protocol->sendActFire();
}
