/*
** EPITECH PROJECT, 2022
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"
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

        if (this->_protocol->handleIncMessages())
            return 0;

        currentMenu->apply();

        if (this->_protocol->shouldGameStart()) {
            this->launchGame();

            // Check if tcp connection still open
            if (*this->_tcpStopFlag)
                return 0;

            // Prepare for next games
            this->_protocol->resetStartGame();
        }

        currentMenu->draw();
    }

    return 0;
}

ClientLobbyProtocol* Client::getProtocol() {
    return this->_protocol;
};
