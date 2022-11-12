#include "LobbyMenu.hpp"
#include "../../../shared/Utilities/Utilities.hpp"
#include "../../../shared/Utilities/ray.hpp"
#include "../../Client.hpp"

LobbyMenu::LobbyMenu(Client* client) {
    SetWindowTitle("R-Type - Lobby");
    this->_client = client;
    this->_startButton = Button("Start", ALIGN_MIDDLE(500), GetScreenHeight() / 2, 500, 200, {255, 255, 255, 255});
};

void LobbyMenu::apply() {
    this->_startButton.apply();

    if (this->_startButton.hasBeenPressed()) {
        this->_client->getProtocol()->sendStart();
        this->_done = true;
        this->_startButton.disable();
    }
};

void LobbyMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    this->_startButton.draw();

    EndDrawing();
};

bool LobbyMenu::getDone() {
    return this->_done;
};
