/*
** EPITECH PROJECT, 2022
** SpriteSystem.cpp
** File description:
** .
*/

#include "SpriteSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include <algorithm>

SpriteSystem::SpriteSystem(Manager* ECS) {
    this->_ECS = ECS;
}

Texture2D SpriteSystem::loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen) {
    const cmrc::file image = this->_fs.open(path);

    const unsigned char* imageBuffer = (unsigned char*)(image.begin());

    Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

    const Rectangle crop{xpos, ypos, xlen, ylen};

    ImageCrop(&sprite, crop);
    ImageResizeNN(&sprite, xlen * 3, ylen * 3);

    Texture2D texture = LoadTextureFromImage(sprite);

    UnloadImage(sprite);

    return texture;
}

AnimationStr* SpriteSystem::loadAnimation(AnimationSheet animationSheet) {
    AnimationStr* animation = new AnimationStr();

    for (int y = 0; y < animationSheet.animHeight; y++) {
        for (int x = 0; x < animationSheet.animWidth; x++) {
            const float xPos = animationSheet.startX + (animationSheet.frameWidth * x) + (animationSheet.separationX * x);
            const float yPos = animationSheet.startY + (animationSheet.frameHeight * y) + (animationSheet.separationY * y);
            Texture2D sprite = this->loadSprite(animationSheet.path, xPos, yPos, animationSheet.frameWidth, animationSheet.frameHeight);
            animation->sequence.push_back(sprite);
        }
    }

    if (animationSheet.reverse) {
        for (int i = animation->sequence.size() - 1; 0 <= i; i--) {
            animation->sequence.push_back(animation->sequence[i]);
        }
    }
    return animation;
}

void SpriteSystem::nextFrame(AnimationStr* animation) {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - animation->timer;

    if (elapsed_seconds.count() > 0.1) {
        animation->index = animation->index + 1;
        animation->timer = now;
        if (animation->index >= animation->sequence.size())
            animation->index = 0;
    }
}

void SpriteSystem::apply() {

    std::vector<int> layers;

    for (auto layer : this->_animationLayers) {
        layers.push_back(layer.first);
    }

    std::sort(layers.begin(), layers.end());

    for (const int layer : layers) {
        for (auto entity : this->_animationLayers[layer]) {

            Animation::Component* component = this->_ECS->getComponent<Animation::Component>(entity.first);
            Position::Component* position = this->_ECS->getComponent<Position::Component>(entity.first);

            AnimationStr* anim = entity.second;

            if (anim == nullptr) {
                throw "Entity does not have a set animation in the Sprite Manager";
            }

            Texture2D frame = anim->sequence[anim->index];

            DrawTexture(frame, position->xPos - frame.width / 2, position->yPos - frame.height / 2 - 40, WHITE);

            this->nextFrame(anim);
        }
    }
}

void SpriteSystem::addAnimation(EntityID ID, Animation::Component* component) {
    this->_animationLayers[component->layer][ID] = this->loadAnimation(SpriteSystem::ANIMATION_SHEET[component->animationID]);
}

std::chrono::time_point<std::chrono::system_clock> getNow() {
    return std::chrono::system_clock::now();
}

std::map<Animation::AnimationID, AnimationSheet> SpriteSystem::ANIMATION_SHEET = {
    {Animation::AnimationID::Orb, {"resources/r-typesheet3.png", 1, 1, 16, 16, 12, 1, 1, 0, 0}},
    {Animation::AnimationID::Vortex, {"resources/r-typesheet30a.png", 1, 3, 31, 31, 3, 1, 2, 0, 0}},
    {Animation::AnimationID::Cluster, {"resources/r-typesheet32.png", 0, 0, 259, 142, 2, 3, 1, 1, 1}},
};