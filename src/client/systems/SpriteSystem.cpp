/*
** EPITECH PROJECT, 2022
** SpriteSystem.cpp
** File description:
** .
*/

#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Manager.hpp"
#include "SpriteSystem.hpp"
#include <algorithm>

#define HAS_KEY(map, key) (map.find(key) != map.end())

SpriteSystem::SpriteSystem(Manager* ECS) {
    this->_ECS = ECS;
}

Texture2D SpriteSystem::loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen,
                                   Animation::Component* component) {
    const cmrc::file image = this->_fs.open(path);

    const unsigned char* imageBuffer = (unsigned char*)(image.begin());

    Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

    const Rectangle crop{xpos, ypos, xlen, ylen};

    ImageCrop(&sprite, crop);
    ImageResizeNN(&sprite, xlen * component->scale, ylen * component->scale);

    Texture2D texture = LoadTextureFromImage(sprite);

    UnloadImage(sprite);

    return texture;
}

void SpriteSystem::loadAnimation(Animation::Component* component) {
    if (HAS_KEY(this->_animations, component->animationID))
        return;

    this->_animations[component->animationID] = {};

    if (!HAS_KEY(Animation::Sheets, component->animationID))
        throw "Now Animation Sheet defined for enum " + component->animationID;

    const Animation::Sheet animationSheet = Animation::Sheets[component->animationID];

    for (int y = 0; y < animationSheet.animHeight; y++) {
        for (int x = 0; x < animationSheet.animWidth; x++) {
            const float xPos = animationSheet.startX + (animationSheet.frameWidth * x) + (animationSheet.separationX * x);
            const float yPos = animationSheet.startY + (animationSheet.frameHeight * y) + (animationSheet.separationY * y);
            Texture2D sprite = this->loadSprite(animationSheet.path, xPos, yPos, animationSheet.frameWidth, animationSheet.frameHeight, component);
            this->_animations[component->animationID].push_back(sprite);
        }
    }

    if (animationSheet.reverse) {
        for (int i = this->_animations[component->animationID].size() - 1; 0 <= i; i--) {
            this->_animations[component->animationID].push_back(this->_animations[component->animationID][i]);
        }
    }
}

void SpriteSystem::nextFrame(Animation::Component* animation) {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - animation->timer;

    if (elapsed_seconds.count() > 0.1) {
        animation->index = animation->index + 1;
        animation->timer = now;
        if (animation->index >= this->_animations[animation->animationID].size())
            animation->index = 0;
    }
}

void SpriteSystem::apply() {
    std::map<int, std::vector<EntityID>> animationLayers;
    std::vector<int> layers;

    for (auto beg = this->_ECS->begin<Animation::Component>(); beg != this->_ECS->end<Animation::Component>(); ++beg) {
        Animation::Component* anim = this->_ECS->getComponent<Animation::Component>(*beg);
        layers.push_back(anim->layer);
        animationLayers[anim->layer].push_back(*beg);
    }

    std::sort(layers.begin(), layers.end());

    for (const int layer : layers) {
        for (auto entity : animationLayers[layer]) {
            Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(entity);
            Position::Component* position = this->_ECS->getComponent<Position::Component>(entity);

            if (!HAS_KEY(this->_animations, animation->animationID))
                this->loadAnimation(animation);

            Texture2D frame = this->_animations[animation->animationID][animation->index];
            Vector2 posVec{position->xPos - frame.width / 2, position->yPos - frame.height / 2 - 40};

            DrawTextureEx(frame, posVec, animation->rotation, 1, WHITE);
            this->nextFrame(animation);
        }
    }
}
