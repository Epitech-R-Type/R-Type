#include "LobbyMenu.hpp"
#include "../../../shared/Utilities/Utilities.hpp"
#include "../../../shared/Utilities/ray.hpp"
#include "../../Client.hpp"

LobbyMenu::LobbyMenu(Client* client) {
    SetWindowTitle("R-Type - Lobby");
    this->_client = client;
    this->_startButton = Button("Start", ALIGN_MIDDLE(500), GetScreenHeight() / 2, 500, 200, WHITE);
    this->_leaveButton = Button("<- Back", 30, GetScreenHeight() * 0.8, 275, 200, WHITE);
};

void LobbyMenu::apply() {
    if (this->_state != State::RUNNING)
        return;

    this->_startButton.apply();
    this->_leaveButton.apply();

    if (this->_startButton.hasBeenPressed()) {
        this->_client->getProtocol()->sendStart();
        this->_state = State::DONE;
        this->_startButton.disable();
    }

    if (this->_leaveButton.hasBeenPressed()) {
        this->_client->getProtocol()->sendLeave();
        this->_state = State::BACK;
        this->_leaveButton.disable();
    }
};

void LobbyMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    this->_startButton.draw();
    this->_leaveButton.draw();
    EndDrawing();
};
