#pragma once

#include "Menu.hpp"

class Client;

class LobbySelectionMenu : public Menu {
public:
    LobbySelectionMenu(Client* client);

    void apply() override;

    void draw() override;

    bool getDone() override;

private:
    bool _done = false;

    Client* _client;
};
