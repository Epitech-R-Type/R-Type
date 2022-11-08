/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "../../WindowsGuard.hpp"

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Serialization.hpp"
#include "../../shared/Utilities/Timer.hpp"
#include "ClientGame.hpp"
#include "raylib.h"

ClientGame::ClientGame(UUIDM uuid, asio::ip::address addr, int port, std::shared_ptr<std::atomic<bool>> stopFlag) {
    this->_isRunning = true;

    // Init com thread
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_entManager = std::make_shared<ECSManager>();

    this->_uuid = uuid;
    this->_musicSystem = std::make_unique<MusicSystem>();

    this->_protocol = std::make_shared<ClientGameProtocol>(this->_incomingMQ, this->_outgoingMQ, this->_entManager, this->_musicSystem, addr,
                                                           asio::ip::port_type(port), this->_uuid);
    this->_stopFlag = stopFlag;

    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, -1);
    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_entManager);
    this->_healthSystem = std::make_unique<HealthSystem>(this->_entManager);
    this->_inputSystem = std::make_unique<PlayerMovementSystem>(this->_protocol);
}

ClientGame::~ClientGame() {
    // Close raylib window
    CloseWindow();

    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();

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
    Timer pingTimer(1);

    while (this->_isRunning && !*(this->_stopFlag)) // Detect window close button or ESC key
    {
        // Send PING command every second
        if (pingTimer.isExpired()) {
            pingTimer.resetTimer();
            this->_protocol->sendPing();
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (this->_protocol->handleCommands())
            break;

        this->_spriteSystem->apply();
        this->_healthSystem->apply();
        this->_musicSystem->apply();
        this->_inputSystem->apply();

        EndDrawing();
    }

    LOG("Game has ended...");
}
