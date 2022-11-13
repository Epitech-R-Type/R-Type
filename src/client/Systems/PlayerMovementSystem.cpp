/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/

#include "PlayerMovementSystem.hpp"
#include "../../shared/ECS/ECSManager.hpp"
#include "../../shared/Utilities/ray.hpp"
#include "../Protocols/ClientGameProtocol.hpp"

PlayerMovementSystem::PlayerMovementSystem(std::shared_ptr<ClientGameProtocol> protocol) {
    this->_protocol = protocol;
}

void PlayerMovementSystem::apply() {
    char directions = 0;
    if (IsKeyDown(KEY_A))
        directions += Move::LEFT;
    if (IsKeyDown(KEY_D))
        directions += Move::RIGHT;
    if (IsKeyDown(KEY_W))
        directions += Move::UP;
    if (IsKeyDown(KEY_S))
        directions += Move::DOWN;
    if (directions != 0)
        this->_protocol->sendActMove(directions);
    if (IsKeyDown(KEY_SPACE))
        this->_protocol->sendActFire();
}
