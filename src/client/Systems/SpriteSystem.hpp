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
#include "../../shared/Systems/ISystem.hpp"

CMRC_DECLARE(client);

// std::chrono::time_point<std::chrono::system_clock> getNow();

typedef std::vector<Texture2D> AnimationArr;

class SpriteSystem : public System {
public:
    SpriteSystem(ECSManager* ECS);

    /**
     * Draw all entities with Animation Components
     * */
    void apply();

    void drawImage(Animation::AnimationID);

private:
    /**
     * Load the part of an image file into a Texture2D object
     * */
    Texture2D loadSprite(Animation::Sheet sheet, const float xpos, const float ypos);

    /**
     * Parses image file to extract all frames of an animation
     * */
    void loadAnimation(Animation::AnimationID id);

    /**
     * Increments currently displayed frame of animation if enough time passed since the last update
     * */
    void nextFrame(Animation::Component* animation);

    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();

    std::map<Animation::AnimationID, AnimationArr> _animations;

    ECSManager* _ECS;
};