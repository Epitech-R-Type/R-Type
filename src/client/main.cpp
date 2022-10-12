#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "./systems/Systems.hpp"

#include "raylib.h"
#include "uuid.h"
#include <iostream>
#include <regex>

Index g_idCounter = 0;

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
    constexpr int screenWidth = 1600;
    constexpr int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "R-Type");

    Manager em;

    SpriteSystem* spriteSystem = new SpriteSystem(&em);
    VelocitySystem* velocitySystem = new VelocitySystem(&em);

    EntityID ent1 = em.newEntity();
    EntityID ent2 = em.newEntity();

    em.addComp<Position::Component>(ent1, {(float)(GetScreenWidth() * (2.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    em.addComp<Animation::Component>(ent1, {Animation::AnimationID::Orb, 1});
    em.addComp<Velocity::Component>(ent1, {0.05, -0.05});

    em.addComp<Position::Component>(ent2, {(float)(GetScreenWidth() * (1.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    em.addComp<Animation::Component>(ent2, {Animation::AnimationID::Vortex, 1});
    em.addComp<Velocity::Component>(ent2, {0.00, -0.05});

    spriteSystem->addAnimation(ent1, em.getComponent<Animation::Component>(ent1));
    spriteSystem->addAnimation(ent2, em.getComponent<Animation::Component>(ent2));
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        spriteSystem->apply();
        velocitySystem->apply();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
