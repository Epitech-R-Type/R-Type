#pragma once

#include "../Components/Button.hpp"
#include "Menu.hpp"

class Client;

class LobbyMenu : public Menu {
public:
    LobbyMenu(Client* client);

    void apply() override;

    void draw() override;

    bool getDone() override;

private:
    Button _startButton;

    bool _done = false;

    Client* _client;
};
