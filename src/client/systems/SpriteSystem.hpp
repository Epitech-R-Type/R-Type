/*
** EPITECH PROJECT, 2022
** SpriteSystem.cpp
** File description:
** .
*/

#pragma once

#include "../../WindowsGuard.hpp"

#include <chrono>
#include <cmrc/cmrc.hpp>
#include <raylib.h>
#include <vector>

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/systems/ISystem.hpp"

CMRC_DECLARE(client);

// std::chrono::time_point<std::chrono::system_clock> getNow();

typedef std::vector<Texture2D> AnimationArr;

struct AnimationSheet {
    std::string path;
    float startX;
    float startY;
    float frameWidth;
    float frameHeight;
    int animWidth;
    int animHeight;
    int separationX;
    int separationY;
    // adds the same frames in reverse to the animation
    bool reverse;
};

class SpriteSystem : public System {
public:
    SpriteSystem(Manager* ECS);

    /**
     * Draw all entities with Animation Components
     * */
    void apply();

    static std::map<Animation::AnimationID, AnimationSheet> ANIMATION_SHEET;

private:
    /**
     * Load the part of an image file into a Texture2D object
     * */
    Texture2D loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen,
                         Animation::Component* animation);

    /**
     * Parses image file to extract all frames of an animation
     * */
    void loadAnimation(Animation::Component* animation);

    /**
     * Increments currently displayed frame of animation if enough time passed since the last update
     * */
    void nextFrame(Animation::Component* animation);

    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();

    std::map<Animation::AnimationID, AnimationArr> _animations;

    Manager* _ECS;
};
