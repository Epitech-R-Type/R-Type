#include "LobbySelectionMenu.hpp"
#include "../../../shared/Utilities/ray.hpp"
#include "../../Client.hpp"

LobbySelectionMenu::LobbySelectionMenu(Client* client) {
    SetWindowTitle("R-Type - Lobby Selection");
    this->_client = client;

    float width = 300;
    float height = 50;
    float yAxis = 100;

    this->_buttonWidth = 300;
    this->_buttonHeight = 50;
    this->_buttonXPos = ALIGN_MIDDLE(width);
    this->_buttonYPos = 100;
    this->_buttonColor = WHITE;

    this->queryLobbies();
};

std::string LobbySelectionMenu::makeLobbyText(LobbyInfo lobby) {
    int maxPlayers = 4;

    return "Lobby " + std::to_string(lobby.id) + ": " + std::to_string(lobby.playerCount) + "/" + std::to_string(maxPlayers);
};

void LobbySelectionMenu::queryLobbies() {
    std::vector<LobbyInfo> lobbies = this->_client->getProtocol()->sendGetLobbies();
    int highestLobbyID = -1;

    this->_lobbies.clear();

    // pushing all existing lobbies into lobby array
    for (LobbyInfo lobby : lobbies) {
        std::string text = this->makeLobbyText(lobby);

        Button button(text, this->_buttonXPos, this->_buttonYPos, this->_buttonWidth, this->_buttonHeight, this->_buttonColor);

        if (lobby.isRunning)
            button.disable();
        if (lobby.id > highestLobbyID)
            highestLobbyID = lobby.id;
        this->_lobbies.push_back({lobby, std::move(button)});
    }

    highestLobbyID++;
    // setting newly created lobby ID
    this->_lobbies.push_back({
        LobbyInfo{highestLobbyID, false, 0},
        Button("Create Lobby", this->_buttonXPos, this->_buttonYPos, this->_buttonWidth, this->_buttonHeight, this->_buttonColor),
    });
}

void LobbySelectionMenu::refreshLobbies() {
    static Timer timer(0.5);

    if (timer.isExpired()) {
        this->queryLobbies();
        timer.resetTimer();
    }
}

void LobbySelectionMenu::apply() {
    this->refreshLobbies();

    for (auto& [lobby, button] : this->_lobbies) {
        button.apply();
        if (button.hasBeenPressed() && this->_client->getProtocol()->sendJoinLobby(lobby.id)) {
            this->_done = true;
            return;
        }
    }
}

void LobbySelectionMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);

    for (auto& [lobby, button] : this->_lobbies) {
        button.draw();
    }

    EndDrawing();
};

bool LobbySelectionMenu::getDone() {
    return this->_done;
};
