/*
** EPITECH PROJECT, 2022
** SpriteSystem.cpp
** File description:
** .
*/

#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/Systems/ISystem.hpp"
#include "../../shared/Utilities/ray.hpp"
#include <cmrc/cmrc.hpp>
#include <vector>

CMRC_DECLARE(client);

typedef std::vector<Texture2D> AnimationArr;

class SpriteSystem : public System {
public:
    SpriteSystem(std::shared_ptr<ECSManager> ECS);

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

    std::shared_ptr<ECSManager> _ECS;
};
