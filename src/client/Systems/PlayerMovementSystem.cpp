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
    int directions = 0;
    if (Ray::IsKeyDown(Ray::KEY_A))
        directions += Move::LEFT;
    if (Ray::IsKeyDown(Ray::KEY_D))
        directions += Move::RIGHT;
    if (Ray::IsKeyDown(Ray::KEY_W))
        directions += Move::UP;
    if (Ray::IsKeyDown(Ray::KEY_S))
        directions += Move::DOWN;
    if (directions != 0)
        this->_protocol->sendActMove(std::to_string(directions));
    if (Ray::IsKeyDown(Ray::KEY_SPACE))
        this->_protocol->sendActFire();
}
