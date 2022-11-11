#include "LobbySelectionMenu.hpp"
#include "../../../shared/Utilities/ray.hpp"
#include "../../Client.hpp"

LobbySelectionMenu::LobbySelectionMenu(Client* client) {
    SetWindowTitle("R-Type - Lobby Selection");
    this->_client = client;

    float width = 300;
    float height = 50;
    float yAxis = 100;

    for (auto& lobby : this->_client->lobbies) {
        std::string text = makeLobbyText(lobby);

        this->_lobbies.push_back(Button(text, ALIGN_MIDDLE(width), yAxis, width, height, {255, 255, 255, 255}));
        yAxis += 100 + height;
    }
};

std::string LobbySelectionMenu::makeLobbyText(int lobby) {
    int maxPlayers = 4;
    int currentPlayers = 0;

    return "Lobby " + std::to_string(lobby) + ": " + std::to_string(currentPlayers) + "/" + std::to_string(maxPlayers);
};

void LobbySelectionMenu::apply() {
    for (auto& button : this->_lobbies) {
        button.apply();
    }
};

void LobbySelectionMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);

    for (auto& button : this->_lobbies) {
        button.draw();
    }

    EndDrawing();
};

bool LobbySelectionMenu::getDone() {
    return this->_done;
};
