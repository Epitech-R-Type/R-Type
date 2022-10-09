#include "../shared/ECS/ECS.hpp"
#include "raylib.h"
#include "uuid.h"
#include <cmrc/cmrc.hpp>
#include <iostream>
#include <regex>

CMRC_DECLARE(client);

#include <iostream>

int main() {
    std::cout << "Client: Hello, World!" << std::endl;

    Manager manager;
    auto& newPlayer(manager.addEntity());

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

    void gameLoop() {}

    void menu() {
        // const uuids::uuid id = uuids::uuid_random_generator{}();
        auto id = uuids::uuid::from_string("d");
    }

    uuids::uuid regexToUUID(const std::string value, std::string regex, std::regex_constants::syntax_option_type constants) {

        const std::regex reg(regex, constants);
        std::smatch match;

        std::regex_search(value.begin(), value.end(), match, reg);

        std::optional<uuids::uuid> maybeClientUUID = uuids::uuid::from_string(match.str());

        return maybeClientUUID.value();
    }

    void conect() {

        const std::string response =
            "CONNECTED serverUUID:251629ad-9075-4a46-8832-83cd415f1147 clientUUID:9f4792ff-6e8f-407c-b1fd-57ab2fd68053 owner:1";

        if (response[response.length() - 1] == '1') {
            this->_isOwner = true;
        }
        this->clientUUID = regexToUUID(response, "/(?<=clientUUID:)(.+)(?=\\s)/", std::regex_constants::egrep);
        this->serverUUID = regexToUUID(response, "/(?<=serverUUID:)(.+)(?=\\s)/", std::regex_constants::egrep);
    }

  private:
    bool _isOwner = false;
    uuids::uuid clientUUID;
    uuids::uuid serverUUID;
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

    newPlayer.deserialize("(Health(2),Armor(2),Position(1,2))");

    std::cout << newPlayer << std::endl;

    newPlayer.deserialize("(Health(40),Armor(4),Position(5,-2))");

    std::cout << newPlayer << std::endl;

    return 0;
}
