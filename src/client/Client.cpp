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

void Client::handleUserCommands() {
    std::optional<std::string> potMsg;

    while ((potMsg = this->_userCommands->pop())) {
        std::string msg = potMsg.value();
        auto splitMsg = Utilities::splitStr(msg, ";");

        if (splitMsg[0] == "Start")
            this->_protocol->sendStart();
        if (splitMsg[0] == "Join") {
            if (splitMsg.size() != 2) {
                ERRORLOG("Please specify lobby to join...");
                continue;
            }

            try {
                int lobby = std::stoi(splitMsg[1]);

                if (lobby < 0)
                    throw;

                this->_protocol->sendJoinLobby(lobby);
            } catch (...) {
                ERRORLOG("Invalid lobby number...");
            }
        }

        // Get_lobbies
        if (splitMsg[0] == "Get_lobbies") {
            auto lobbies = this->_protocol->sendGetLobbies();

            // testing purposes
            for (auto lobby : lobbies) {
                std::cout << "Lobby [" << lobby.id << "] has " << lobby.playerCount << " players connected." << std::endl;
                std::cout << "Is game started ? == " << lobby.isRunning << std::endl;
            }
        }

        // Leave
        if (splitMsg[0] == "Leave") {
            this->_protocol->sendLeave();
            continue;
        }
    }
}

Stages Client::advanceStage(Stages stage, std::unique_ptr<Menu>& currentMenu) {
    if (currentMenu->getDone()) {
        switch (stage) {
            case Stages::CONNECTION: {
                currentMenu = std::make_unique<LobbySelectionMenu>(this);
                return Stages::ROOMSELECTION;
            }
            case Stages::ROOMSELECTION:
                currentMenu = std::make_unique<LobbyMenu>(this);
                return Stages::ROOM;
            default:
                return stage;
        }
    };
    return stage;
}

int Client::mainLoop() {

    Stages stage = this->_connected ? Stages::ROOMSELECTION : Stages::CONNECTION;

    std::unique_ptr<Menu> currentMenu;

    if (this->_connected)
        currentMenu = std::make_unique<LobbySelectionMenu>(this);
    else
        currentMenu = std::make_unique<ConnectionMenu>(this);

    while (true) {
        stage = this->advanceStage(stage, currentMenu);

        if (this->_protocol->handleIncMessages())
            return 0;

        this->handleUserCommands();

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
