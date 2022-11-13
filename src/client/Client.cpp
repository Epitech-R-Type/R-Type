/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/ECSManager.hpp"
#include "../shared/MessageQueue/MessageQueue.hpp"
#include "../shared/Utilities/ray.hpp"
#include "GUI/Menus/ConnectionMenu.hpp"
#include "GUI/Menus/LobbyMenu.hpp"
#include "GUI/Menus/LobbySelectionMenu.hpp"

Client::Client() {
    this->_tcpStopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_protocol = new ClientLobbyProtocol(this->_tcpStopFlag);

    this->_lobbyRunning = true;
    this->_connected = false;

    // Gangster Workaround to insure same comptype order client
    Utilities::createCompPoolIndexes();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    while (!IsWindowReady()) {
        //
    }

    SetTargetFPS(40);

    this->_ECS = std::make_shared<ECSManager>();
    this->_spriteSystem = std::make_unique<SpriteSystem>(this->_ECS);

    EntityID id = this->_ECS->newEntity();

    this->_ECS->addComp<Position::Component>(id, {0, 0});
    this->_ECS->addComp<Animation::Component>(id, {Animation::AnimationID::MenuBackground, 0});
    this->_ECS->addComp<Velocity::Component>(id, {2, 0});
}

int Client::launchGame() {
    if (!this->_connected)
        return 84;

    // Note: For performance reasons we could free the lobby ecs before launching the game
    this->_game = new ClientGame(this->_protocol->getUUID(), this->_protocol->getServerIp(), this->_protocol->getServerPort(), this->_tcpStopFlag);
    this->_game->init();
    this->_game->mainLoop();

    delete this->_game;

    return 0;
}

int Client::connect(std::string serverIP, int port) {
    const int res = this->_protocol->connect(serverIP, port);
    this->_connected = this->_protocol->isConnected();
    return res;
}

void Client::advanceStage(std::unique_ptr<Menu>& currentMenu) {
    State state = currentMenu->getState();

    if (state == State::DONE) {
        switch (this->_currentStage) {
            case Stages::CONNECTION: {
                currentMenu = std::make_unique<LobbySelectionMenu>(this);
                this->_currentStage = Stages::ROOMSELECTION;
                return;
            }
            case Stages::ROOMSELECTION:
                currentMenu = std::make_unique<LobbyMenu>(this);
                this->_currentStage = Stages::ROOM;
                return;
            default:
                return;
        }
    } else if (state == State::BACK) {
        switch (this->_currentStage) {
            case Stages::ROOM: {
                currentMenu = std::make_unique<LobbySelectionMenu>(this);
                this->_currentStage = Stages::ROOMSELECTION;
            }
            default:
                ERRORLOG("Invalid stage: " << this->_currentStage);
        }
    };
}

int Client::mainLoop() {
    std::unique_ptr<Menu> currentMenu;

    if (this->_connected) {
        currentMenu = std::make_unique<LobbySelectionMenu>(this);
        this->_currentStage = Stages::ROOMSELECTION;
    } else {
        currentMenu = std::make_unique<ConnectionMenu>(this);
        this->_currentStage = Stages::CONNECTION;
    }

    while (true) {
        this->advanceStage(currentMenu);

        if (this->_protocol->handleIncMessages()) {
            return 1;
        }

        currentMenu->apply();

        if (this->_protocol->shouldGameStart()) {
            this->launchGame();

            // Check if tcp connection still open
            if (this->_tcpStopFlag.get()->load() == true)
                return 1;

            currentMenu = std::make_unique<LobbySelectionMenu>(this);
            this->_currentStage = Stages::ROOMSELECTION;

            // Prepare for next games
            this->_protocol->resetStartGame();
        }

        currentMenu->draw();
        this->_spriteSystem->apply();
    }

    return 0;
}

void Client::reset() {
    this->_tcpStopFlag = std::make_shared<std::atomic<bool>>(false);
    this->_protocol = new ClientLobbyProtocol(this->_tcpStopFlag);
    this->_connected = false;
    this->_lobbyRunning = true;
};

ClientLobbyProtocol* Client::getProtocol() {
    return this->_protocol;
};
