/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Serialization.hpp"
#include "ClientGame.hpp"
#include "raylib.h"

ClientGame::ClientGame() {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    // this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);

    this->_entManager = std::make_shared<ECSManager>();
    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_entManager);
    this->_velocitySystem = std::make_unique<VelocitySystem>(this->_entManager);
    this->_playerMovementSystem = std::make_unique<PlayerMovementSystem>(this->_entManager);
    this->_healthSystem = std::make_unique<HealthSystem>(this->_entManager);
}

ClientGame::~ClientGame() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    // this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

void ClientGame::init() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    // this->_player = getPlayer();

    this->_playerMovementSystem->setPlayer(this->_player);
    this->_healthSystem->setPlayer(this->_player);
}

void ClientGame::mainLoop() {
    this->init();

    while (this->_entManager->entityIsActive(this->_player)) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);

        this->_spriteSystem->apply();
        this->_velocitySystem->apply();
        this->_playerMovementSystem->apply();
        this->_healthSystem->apply();

        EndDrawing();
    }
}
