#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "../shared/systems/UUID.hpp"
#include "./systems/Systems.hpp"
#include "raylib.h"
#include <iostream>

Index g_idCounter = 0;

int main() {
    constexpr int screenWidth = 1600;
    constexpr int screenHeight = 900;

    UUID x;

    std::cout << x.toString() << " " << (x == x) << std::endl;

    std::cout << UUID("sdqsdnqos 5acc0e06-2212-4c42-9ade-de53e9cdd1f2 1").toString() << std::endl;

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
    em.addComp<Animation::Component>(ent2, {Animation::AnimationID::Cluster, 1});

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
