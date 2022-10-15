/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Components.hpp"
#include "ClientGame.hpp"
#include "factories.hpp"
#include "raylib.h"

ClientGame::ClientGame() {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);

    this->_entManager = new Manager();
    this->_spriteSystem = new SpriteSystem(this->_entManager);
    this->_velocitySystem = new VelocitySystem(this->_entManager);

    // ClientGame will take the player EntityID as consturctor param, replace y Server message
    this->_player = makePlayer(this->_entManager, this->_spriteSystem);

    this->_playerMovementSystem = new PlayerMovementSystem(this->_entManager);

    this->_playerMovementSystem->setPlayer(this->_player);

    this->_healthSystem = new HealthSystem(this->_entManager);
    this->_healthSystem->setPlayer(this->_player);

    this->_armamentSystem = new ArmamentSystem(this->_entManager);
    this->_armamentSystem->setPlayer(this->_player);
    this->_armamentSystem->setSpriteSystem(this->_spriteSystem);
}

ClientGame::~ClientGame() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

void ClientGame::init() {
    srand(time(0));

    makeEnemy(this->_entManager, this->_spriteSystem);
    makeEnemy(this->_entManager, this->_spriteSystem);
    makeEnemy(this->_entManager, this->_spriteSystem);
    makeEnemy(this->_entManager, this->_spriteSystem);

    makeEndboss(this->_entManager, this->_spriteSystem);
}

void ClientGame::mainLoop() {
    std::cout << "Entering main loop()" << std::endl;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);

        this->_spriteSystem->apply();
        this->_velocitySystem->apply();
        this->_playerMovementSystem->apply();
        this->_healthSystem->apply();
        this->_armamentSystem->apply();

        EndDrawing();
    }
}
