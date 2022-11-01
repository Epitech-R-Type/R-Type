/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include "../../shared/Systems/ISystem.hpp"
class ClientGameProtocol;

class PlayerMovementSystem : public System {
public:
    PlayerMovementSystem(std::shared_ptr<ClientGameProtocol> protocol);

    void apply();

private:
    std::shared_ptr<ClientGameProtocol> _protocol;
};