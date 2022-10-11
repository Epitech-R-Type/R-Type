/*
** EPITECH PROJECT, 2022
** SpriteManager.cpp
** File description:
** .
*/

#include "./SpriteManager.hpp"

Texture2D SpriteManager::loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen) {
    const cmrc::file image = this->_fs.open(path);

    const unsigned char* imageBuffer = (unsigned char*)(image.begin());

    Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

    const Rectangle crop{xpos, ypos, xlen, ylen};

    ImageCrop(&sprite, crop);

    Texture2D texture = LoadTextureFromImage(sprite);

    UnloadImage(sprite);

    return texture;
}

Animation SpriteManager::loadAnimation(const std::string path, const float startX, const float startY, const float frameWidth,
                                       const float frameHeight, const int animWidth, const int animHeight, const int separation) {
    Animation animation;
    for (int y = 0; y < animHeight; y++) {
        for (int x = 0; x < animWidth; x++) {
            const float xPos = startX + (frameWidth * x) + (separation * x);
            const float yPos = startY + (frameHeight * y) + (separation * y);
            Texture2D sprite = this->loadSprite(path, xPos, yPos, frameWidth, frameHeight);
            animation.sequence.push_back(sprite);
        }
    }
    return animation;
}

void SpriteManager::nextFrame(Animation* animation) {
    const auto now = getNow();
    std::chrono::duration<double> elapsed_seconds = now - animation->timer;

    if (elapsed_seconds.count() > 0.1) {
        animation->index = animation->index + 1;
        animation->timer = now;
        if (animation->index >= animation->sequence.size())
            animation->index = 0;
    }
}

void SpriteManager::draw(Manager* ECS) {
    for (auto beg = ECS->begin<AnimationSet::Component>(); ECS->begin<AnimationSet::Component>() != ECS->end<AnimationSet::Component>(); ++beg) {
        EntityID id = *beg;
        AnimationSet::Component* component = ECS->getComponent<AnimationSet::Component>(id);
        Position::Component* position = ECS->getComponent<Position::Component>(id);

        Animation* anim = &this->_animationLayers[component->layer][id];

        Texture2D frame = anim->sequence[anim->index];

        DrawTexture(frame, position->xPos - frame.width / 2, position->yPos - frame.height / 2 - 40, WHITE);

        this->nextFrame(anim);
    }
}

void SpriteManager::addAnimation(EntityID ID, AnimationSet::Component* component) {
    this->_animationLayers[component->layer][ID] = this->loadAnimation("resources/r-typesheet3.png", 1, 1, 16, 16, 12, 1, 1);
}