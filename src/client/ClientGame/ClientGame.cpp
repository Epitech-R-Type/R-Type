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

ClientGame::ClientGame(Utilities::UUID uuid, asio::ip::address serverAddr, int serverUdpPort, std::shared_ptr<std::atomic<bool>> tcpStopFlag) {
    this->_isRunning = true;

    // Init com thread
    this->_incomingMQ = std::make_shared<MessageQueue<Message<ByteBuf>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<ByteBuf>>>();
    this->_entManager = std::make_shared<ECSManager>();

    this->_uuid = uuid;
#ifndef WIN32_LEAN_AND_MEAN
    this->_musicSystem = std::make_unique<MusicSystem>();
#endif
    int clientUdpPort = UDP_PORT;
    while (!Utilities::isPortAvailable(clientUdpPort))
        clientUdpPort++;

    this->_protocol = std::make_shared<ClientGameProtocol>(this->_incomingMQ, this->_outgoingMQ, this->_entManager, this->_musicSystem, serverAddr,
                                                           asio::ip::port_type(serverUdpPort), this->_uuid);
    this->_tcpStopFlag = tcpStopFlag;
    this->_udpStopFlag = std::make_shared<std::atomic<bool>>(false);

    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_udpStopFlag, -1);
    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_entManager);
    this->_healthSystem = std::make_unique<HealthSystem>(this->_entManager);
    this->_inputSystem = std::make_unique<PlayerMovementSystem>(this->_protocol);
}

ClientGame::~ClientGame() {
    // Close raylib window

    // Signal thread to stop and join thread
    this->_udpStopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

void ClientGame::init() {
    // Send here command
    this->_protocol->sendHere();

    this->_healthSystem->setPlayer(this->_player);
}

void ClientGame::mainLoop() {
    Timer pingTimer(1); // Frequency at which ping command is sent

    while (this->_isRunning && !*(this->_tcpStopFlag)) // Detect window close button or ESC key
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

#ifndef WIN32_LEAN_AND_MEAN
        this->_musicSystem->apply();
#endif

        this->_inputSystem->apply();

        EndDrawing();
    }

    LOG("Game has ended...");
}
