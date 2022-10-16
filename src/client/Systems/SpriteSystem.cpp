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

SpriteSystem::SpriteSystem(ECSManager* ECS) {
    this->_ECS = ECS;
}

Texture2D SpriteSystem::loadSprite(Animation::Sheet sheet, const float xpos, const float ypos) {
    const cmrc::file image = this->_fs.open(sheet.path);

    const unsigned char* imageBuffer = (unsigned char*)(image.begin());

    Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

    const Rectangle crop{xpos, ypos, sheet.frameWidth, sheet.frameHeight};

    ImageCrop(&sprite, crop);

    Texture2D texture = LoadTextureFromImage(sprite);

    UnloadImage(sprite);

    return texture;
}

void SpriteSystem::loadAnimation(Animation::AnimationID id) {
    if (HAS_KEY(this->_animations, id))
        return;

    this->_animations[id] = {};

    if (!HAS_KEY(Animation::Sheets, id))
        throw "Now Animation Sheet defined for enum " + id;

    const Animation::Sheet animationSheet = Animation::Sheets[id];

    for (int y = 0; y < animationSheet.animHeight; y++) {
        for (int x = 0; x < animationSheet.animWidth; x++) {
            const float xPos = animationSheet.startX + (animationSheet.frameWidth * x) + (animationSheet.separationX * x);
            const float yPos = animationSheet.startY + (animationSheet.frameHeight * y) + (animationSheet.separationY * y);
            Texture2D sprite = this->loadSprite(animationSheet, xPos, yPos);
            this->_animations[id].push_back(sprite);
        }
    }

    if (animationSheet.reverse) {
        for (int i = this->_animations[id].size() - 1; 0 <= i; i--) {
            this->_animations[id].push_back(this->_animations[id][i]);
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
                this->loadAnimation(animation->animationID);

            Texture2D frame = this->_animations[animation->animationID][animation->index];
            Vector2 posVec{position->x, position->y};
            DrawTextureEx(frame, posVec, animation->rotation, animation->scale, WHITE);
            this->nextFrame(animation);
        }
    }
}

void SpriteSystem::drawImage(Animation::AnimationID id) {

    if (!HAS_KEY(this->_animations, id)) {
        this->loadAnimation(id);
    }

    Texture2D frame = this->_animations[id][0];
    DrawTextureEx(frame, {0, 0}, 0, 1.0 + 639.0 / GetScreenHeight(), WHITE);
};