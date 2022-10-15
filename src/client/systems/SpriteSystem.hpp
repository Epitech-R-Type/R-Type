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

std::chrono::time_point<std::chrono::system_clock> getNow();

struct AnimationStr {
    std::vector<Texture2D> sequence{};
    int index = 0;
    std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
};

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
     * Load the part of an image file into a Texture2D object
     * */
    Texture2D loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen);

    /**
     * Parses image file to extract all frames of an animation
     * */
    AnimationStr* loadAnimation(AnimationSheet animationSheet);

    /**
     * Increments currently displayed frame of animation if enough time passed since the last update
     * */
    void nextFrame(AnimationStr* animation);

    /**
     * Draw all entities with Animation Components
     * */
    void apply();

    /**
     * Adds an animation to the animation storing struct
     * the entityID and Layer value in the component are used to access
     * the linked animation
     * */
    void addAnimation(EntityID ID, Animation::Component* component);
    static std::map<Animation::AnimationID, AnimationSheet> ANIMATION_SHEET;

private:
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
    std::map<int, std::map<EntityID, AnimationStr*>> _animationLayers;
    Manager* _ECS;
};
