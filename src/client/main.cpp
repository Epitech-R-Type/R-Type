#include "../shared/ECS/ECS.hpp"
#include "raylib.h"
#include <cmrc/cmrc.hpp>
#include <iostream>

CMRC_DECLARE(client);

class GameManager {
  public:
    GameManager() = default;
    ~GameManager() = default;

    Texture2D loadSprite(const std::string path, const unsigned int xpos, const unsigned int ypos, const unsigned int xlen, const unsigned int ylen) {

        const cmrc::file image = this->_fs.open(path);

        const unsigned char* imageBuffer = (unsigned char*)(image.begin());

        Image sprite = LoadImageFromMemory("png", imageBuffer, image.size());

        ImageCrop(&sprite, (Rectangle){xpos, ypos, xlen, ylen});

        Texture2D texture = LoadTextureFromImage(sprite);

        UnloadImage(sprite);

        return texture;
    }

  private:
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "R-Type");

    GameManager* gameManager = new GameManager();

    Texture2D texture = gameManager->loadSprite("resources/r-typesheet1.png", 300, 60, 50, 50);
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2 - 40, WHITE);
        DrawRectangleLines(screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2 - 40, texture.width, texture.height, DARKGRAY);

        DrawText("We are drawing only one texture from various images composed!", 240, 350, 10, DARKGRAY);
        DrawText("Source images have been cropped, scaled, flipped and copied one over the other.", 190, 370, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Texture unloading

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
