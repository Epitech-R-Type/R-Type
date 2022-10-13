/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** PlayerMovementSystem
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include <chrono>
#include <raylib.h>

#include "../../shared/ECS/ECS.hpp"
#include "../../shared/systems/ISystem.hpp"

std::chrono::time_point<std::chrono::system_clock> getNow();

class PlayerMovementSystem : public System {
public:
    PlayerMovementSystem(Manager* ECS);
    void apply();
    void setPlayer(EntityID player);

private:
    SpriteSystem* _spriteSystem;
    EntityID _player;
    Manager* _ECS;
    std::chrono::time_point<std::chrono::system_clock> _timer = std::chrono::system_clock::now();
};