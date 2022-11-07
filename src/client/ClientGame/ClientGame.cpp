/*
** EPITECH PROJECT, 2022
** ClientGame.cpp
** File description:
** .*
*/

#include "ClientGame.hpp"
#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Serialization.hpp"
#include "../../shared/Utilities/ray.hpp"

ClientGame::ClientGame(UUIDM uuid, asio::ip::address addr, int serverUdpPort) {
    this->_isRunning = true;

    // Init com thread
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_entManager = std::make_shared<ECSManager>();

    this->_uuid = uuid;
    this->_musicSystem = std::make_unique<MusicSystem>();

    int clientUdpPort = UDP_PORT;
    while (Utilities::isPortAvailable(clientUdpPort))
        clientUdpPort++;

    this->_protocol = std::make_shared<ClientGameProtocol>(this->_incomingMQ, this->_outgoingMQ, this->_entManager, this->_musicSystem, addr,
                                                           asio::ip::port_type(serverUdpPort), this->_uuid);
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);

    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag, clientUdpPort);
    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_entManager);
    this->_healthSystem = std::make_unique<HealthSystem>(this->_entManager);
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
    Ray::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    while (!Ray::IsWindowReady()) {
        //
    }

    // Send here command
    this->_protocol->sendHere();

    this->_healthSystem->setPlayer(this->_player);
}

void ClientGame::mainLoop() {
    while (this->_isRunning) // Detect window close button or ESC key
    {
        Ray::BeginDrawing();

        Ray::ClearBackground(Ray::BLACK);

        this->_protocol->handleCommands();
        this->_spriteSystem->apply();
        this->_healthSystem->apply();
        this->_musicSystem->apply();
        this->_inputSystem->apply();

        Ray::EndDrawing();
    }
}
