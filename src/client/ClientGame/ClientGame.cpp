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

ClientGame::ClientGame(ECSManager* ECS, SpriteSystem* spriteSystem) {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<std::string>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<std::string>>();

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);

    this->_entManager = ECS;

    this->_spriteSystem = spriteSystem;
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

    this->_hitboxSystem = new HitboxSystem(this->_entManager);
    this->_janitorSystem = new JanitorSystem(this->_entManager);
}

ClientGame::~ClientGame() {
    // Delete com thread
    delete this->_udpComThread;
    delete this->_hitboxSystem;
    delete this->_janitorSystem;
    delete this->_armamentSystem;
    delete this->_healthSystem;
    delete this->_playerMovementSystem;
    delete this->_velocitySystem;
}

void ClientGame::init() {
    srand(time(0));

    makeEndboss(this->_entManager, this->_spriteSystem);
}

void ClientGame::mainLoop() {
    std::chrono::time_point<std::chrono::system_clock> timer;

    while (this->_entManager->entityIsActive(this->_player)) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);

        this->_spriteSystem->apply();
        this->_velocitySystem->apply();
        this->_playerMovementSystem->apply();
        this->_healthSystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();

        // Always last
        this->_janitorSystem->apply();

        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - timer;

        // Convert to milliseconds
        if (elapsed_seconds.count() > 0.5) {
            makeEnemy(this->_entManager, this->_spriteSystem);
            timer = getNow();
        }

        EndDrawing();
    }

    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();
}
