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
    this->_connected = false;
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

void Client::connect() {
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // For some reason initializer list initialization wasn't working
    // this->_protocol = new LobbyProtocol(this->_incomingMQ, this->_outgoingMQ);

    // Init tcp com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_comThread = new std::thread(tcp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);
}

int Client::mainLoop() {

    while (this->_lobbyRunning && !WindowShouldClose()) {

        if (IsKeyPressed(KEY_ENTER) && this->_connected)
            this->launchGame();

        if (IsKeyPressed(KEY_C) && !this->_connected) {
            this->connect();
            this->_connected = true;
        }
        BeginDrawing();

        ClearBackground(BLACK);
        this->_spriteSystem->drawImage(Animation::AnimationID::Lost);

        EndDrawing();
    }

    return 0;
}