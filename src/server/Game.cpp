/*
** EPITECH PROJECT, 2022
** Game.cpp
** File description:
** .
*/

#include "Game.hpp"
#include "../WindowsGuard.hpp"

#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/Serialization.hpp"
#include "Systems/Factory.hpp"

Game::Game()
    : _isRunning(true) {
    // Construct messaging queues
    this->_incomingMQ = std::make_shared<MessageQueue<Message<std::string>>>();
    this->_outgoingMQ = std::make_shared<MessageQueue<Message<std::string>>>();

    // Init com thread
    this->_stopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_udpComThread = new std::thread(udp_communication_main, this->_incomingMQ, this->_outgoingMQ, this->_stopFlag);

    this->_entManager = new ECSManager();

    this->_velocitySystem = new VelocitySystem(this->_entManager);
    this->_armamentSystem = new ArmamentSystem(this->_entManager);
    this->_hitboxSystem = new HitboxSystem(this->_entManager);
    this->_janitorSystem = new JanitorSystem(this->_entManager);
}

Game::~Game() {
    // Signal thread to stop and join thread
    this->_stopFlag->store(true);
    this->_udpComThread->join();

    // Delete com thread
    delete this->_udpComThread;
}

void Game::init() {
    srand(time(0));
}

int Game::mainLoop() {
    std::cout << "Entering main loop()" << std::endl;

    std::chrono::time_point<std::chrono::system_clock> timer;
    Factory::Enemy::makeEnemy(this->_entManager);

    while (this->_isRunning) {
        this->_velocitySystem->apply();
        this->_armamentSystem->apply();
        this->_hitboxSystem->apply();

        // Always last
        this->_janitorSystem->apply();

        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - timer;

        // Convert to milliseconds
        if (elapsed_seconds.count() > 0.5) {
            Factory::Enemy::makeEnemy(this->_entManager);
            timer = getNow();
        }
    }

    return 0;
}
