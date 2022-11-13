#pragma once

#include "../Components/Button.hpp"
#include "Menu.hpp"

class Client;

class LobbyMenu : public Menu {
public:
    LobbyMenu(Client* client);

    void apply() override;

    void draw() override;

private:
    Button _startButton;

    Button _leaveButton;

    Client* _client;
};
