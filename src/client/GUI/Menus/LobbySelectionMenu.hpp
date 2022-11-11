#pragma once

#include "../Components/Button.hpp"
#include "Menu.hpp"
#include <vector>

#define ALIGN_MIDDLE(width) ((GetScreenWidth() - width) / 2)

class Client;

class LobbySelectionMenu : public Menu {
public:
    LobbySelectionMenu(Client* client);

    void apply() override;

    void draw() override;

    bool getDone() override;

private:
    std::string makeLobbyText(int lobby);

    bool _done = false;

    Client* _client;

    std::vector<Button> _lobbies;
};
