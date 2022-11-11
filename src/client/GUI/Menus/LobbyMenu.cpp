#include "LobbyMenu.hpp"
#include "../../../shared/Utilities/ray.hpp"

LobbyMenu::LobbyMenu(Client* client) {
    SetWindowTitle("R-Type - Lobby");
    this->_client = client;
};

void LobbyMenu::apply(){};

void LobbyMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    EndDrawing();
};

bool LobbyMenu::getDone() {
    return this->_done;
};
