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

ClientGame::ClientGame(UUIDM uuid, asio::ip::address addr, int port) {
    this->_isRunning = true;

    // Init com thread
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_entManager = std::make_shared<ECSManager>();

    this->_uuid = uuid;
    this->_protocol =
        std::make_shared<ClientGameProtocol>(this->_incomingMQ, this->_outgoingMQ, this->_entManager, addr, asio::ip::port_type(port), this->_uuid);
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread =
        new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, -1); // Bind to available port

    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_entManager);
    this->_healthSystem = std::make_unique<HealthSystem>(this->_entManager);
    this->_musicSystem = std::make_unique<MusicSystem>(0);
    this->_inputSystem = std::make_unique<PlayerMovementSystem>(this->_protocol);
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

    // Send here command
    this->_protocol->sendHere();

    this->_healthSystem->setPlayer(this->_player);
}

void ClientGame::mainLoop() {
    while (this->_isRunning) // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);

        this->_protocol->handleCommands();
        this->_spriteSystem->apply();
        this->_healthSystem->apply();
        this->_musicSystem->apply();
        this->_inputSystem->apply();

        EndDrawing();
    }
}
