/*
** EPITECH PROJECT, 2022
** SpriteManager.cpp
** File description:
** .
*/

#pragma once

#include <chrono>
#include <cmrc/cmrc.hpp>
#include <raylib.h>
#include <vector>

#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"

CMRC_DECLARE(client);

std::chrono::time_point<std::chrono::system_clock> getNow() {
    return std::chrono::system_clock::now();
}

struct Animation {
    std::vector<Texture2D> sequence;
    int index = 0;
    std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
};

// struct animationParams {
//     float xOffset;
//     float yOffset;
//     float frameWidth;
//     float frameHeight;
//     int animationMatrixWidth;
//     int animationMatrixHeight;
//     int frameSeparation;
// };

class SpriteManager {
public:
    Texture2D loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen);

    Animation loadAnimation(const std::string path, const float startX, const float startY, const float frameWidth, const float frameHeight,
                            const int animWidth, const int animHeight, const int separation);
    void nextFrame(Animation* animation);

    void draw(Manager* ECS);

    void addAnimation(EntityID ID, AnimationSet::Component* component);

private:
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
    std::map<int, std::map<EntityID, Animation>> _animationLayers;
};