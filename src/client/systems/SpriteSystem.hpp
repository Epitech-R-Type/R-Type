/*
** EPITECH PROJECT, 2022
** SpriteSystem.cpp
** File description:
** .
*/

#pragma once

#include <chrono>
#include <cmrc/cmrc.hpp>
#include <raylib.h>
#include <vector>

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"

CMRC_DECLARE(client);

std::chrono::time_point<std::chrono::system_clock> getNow();

struct AnimationStr {
    std::vector<Texture2D> sequence{};
    int index = 0;
    std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
};

class SpriteSystem {
public:
    Texture2D loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen);

    AnimationStr* loadAnimation(const std::string path, const float startX, const float startY, const float frameWidth, const float frameHeight,
                                const int animWidth, const int animHeight, const int separation);
    void nextFrame(AnimationStr* animation);

    void draw(Manager* ECS);

    void addAnimation(EntityID ID, Animation::Component* component);

private:
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
    std::map<int, std::map<EntityID, AnimationStr*>> _animationLayers;
};
