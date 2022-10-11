#pragma once

#include <vector>
#include <raylib.h>
#include <cmrc/cmrc.hpp>
#include <chrono>

CMRC_DECLARE(client);

std::chrono::time_point<std::chrono::system_clock> getNow() {
    return std::chrono::system_clock::now();
}
struct Animation {
    std::vector<Texture2D> sequence;
    int index = 0;
    std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
};

struct animationParams {
    float xOffset;
    float yOffset;
    float frameWidth;
    float frameHeight;
    int animationMatrixWidth;
    int animationMatrixHeight;
    int frameSeparation;
};

class SpriteManager
{
    public:

        Texture2D loadSprite(const std::string path, const float xpos, const float ypos, const float xlen, const float ylen) {

            const cmrc::file image = this->_fs.open(path);

            const unsigned char* imageBuffer = (unsigned char*)(image.begin());
            
            Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

            const Rectangle crop{xpos, ypos, xlen, ylen};

            ImageCrop(&sprite, crop);

            Texture2D texture = LoadTextureFromImage(sprite);

            UnloadImage(sprite);

            return texture;
        }

        Animation loadAnimation(const std::string path, const float startX, const float startY, const float frameWidth, const float frameHeight,
                            const int animWidth, const int animHeight, const int separation) {
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

        static void nextFrame(Animation* animation) {
            const auto now = getNow();
            std::chrono::duration<double> elapsed_seconds = now - animation->timer;

            if (elapsed_seconds.count() > 0.1) {
                animation->index = animation->index + 1;
                animation->timer = now;
                if (animation->index >= animation->sequence.size())
                    animation->index = 0;
            }
        }

        void updateAnimations() {
            for (auto animationLayer : this->_animations) {
                for (auto animation : animationLayer.second) {
                    SpriteManager::nextFrame(&animation);
                }
            }
        }

    private:
        cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
        std::map<int, std::vector<Animation>> _animations;
};