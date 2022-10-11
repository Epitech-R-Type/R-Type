#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/ECS.hpp"
#include "SpriteManager.hpp"
#include "raylib.h"
#include "uuid.h"
#include <iostream>
#include <regex>

class GameManager {
public:
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
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "R-Type");

    GameManager* gameManager = new GameManager();
    SpriteManager* spriteManager = new SpriteManager();

    Manager em;

    EntityID ent1 = em.newEntity();
    EntityID ent2 = em.newEntity();

    em.addComp<Position::Component>(ent1, {50, 50});
    em.addComp<AnimationSet::Component>(ent1, {AnimationSet::AnimationID::Orb, 1});

    em.addComp<Position::Component>(ent2, {200, 200});
    em.addComp<AnimationSet::Component>(ent2, {AnimationSet::AnimationID::Orb, 1});

    spriteManager->addAnimation(ent1, em.getComponent<AnimationSet::Component>(ent1));
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        spriteManager->draw(&em);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
