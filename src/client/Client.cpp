/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/Manager.hpp"
#include "raylib.h"

Client::Client() {
    this->_ECS = new ECSManager();
    this->_spriteSystem = new SpriteSystem(this->_ECS);
    this->_lobbyRunning = true;
}

int Client::launchGame() {
    // Note: For performance reasons we could free the lobby ecs before launching the game
    this->_game = new ClientGame(this->_ECS, this->_spriteSystem);
    this->_game->init();
    this->_game->mainLoop();
    delete this->_ECS;
    delete this->_game;

    this->_ECS = new ECSManager();
    this->_spriteSystem = new SpriteSystem(this->_ECS);

    return 0;
}

int Client::mainLoop() {

    while (this->_lobbyRunning && !WindowShouldClose()) {

        if (IsKeyPressed(KEY_ENTER))
            this->launchGame();

        BeginDrawing();

        ClearBackground(BLACK);
        this->_spriteSystem->drawImage(Animation::AnimationID::Lost);

        EndDrawing();
    }

    return 0;
}

int Client::setup() {
    return 0;
}
