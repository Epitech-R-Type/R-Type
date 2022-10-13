/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "ClientGame.hpp"
#include "../../shared/ECS/Components.hpp"
#include "raylib.h"

EntityID makePlayer(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID player = ECS->newEntity();

    ECS->addComp<Position::Component>(player, {(float)(GetScreenWidth() * (2.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    ECS->addComp<Animation::Component>(player, {Animation::AnimationID::Vortex, 2});

    spriteSystem->addAnimation(player, ECS->getComponent<Animation::Component>(player));

    return player;
}

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

    InitWindow(1600, 900, "R-Type");

    // will take the player EntityID as consturctor param
    this->_player = makePlayer(this->_entManager, this->_spriteSystem);

    this->_playerMovementSystem = new PlayerMovementSystem(this->_entManager);

    this->_playerMovementSystem->setPlayer(this->_player);
}

ClientGame::~ClientGame() {
    // Delete com thread
    delete this->_udpComThread;
}

void ClientGame::init() {
    EntityID ent1 = this->_entManager->newEntity();
    EntityID ent2 = this->_entManager->newEntity();

    this->_entManager->addComp<Position::Component>(ent1, {(float)(GetScreenWidth() * (2.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    this->_entManager->addComp<Animation::Component>(ent1, {Animation::AnimationID::Orb, 1});
    this->_entManager->addComp<Velocity::Component>(ent1, {0.05, -0.05});
    this->_entManager->addComp<Player::Component>(ent1, {true});

    this->_entManager->addComp<Position::Component>(ent2, {(float)(GetScreenWidth() * (1.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    this->_entManager->addComp<Animation::Component>(ent2, {Animation::AnimationID::Cluster, 1});

    this->_spriteSystem->addAnimation(ent1, this->_entManager->getComponent<Animation::Component>(ent1));
    this->_spriteSystem->addAnimation(ent2, this->_entManager->getComponent<Animation::Component>(ent2));
}

void ClientGame::mainLoop() {
    std::cout << "Entering main loop()" << std::endl;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        this->_spriteSystem->apply();
        this->_velocitySystem->apply();
        this->_playerMovementSystem->apply();

        int x = this->_entManager->getComponent<Position::Component>(this->_player)->xPos;
        int y = this->_entManager->getComponent<Position::Component>(this->_player)->yPos;

        EndDrawing();
    }

    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();
}
