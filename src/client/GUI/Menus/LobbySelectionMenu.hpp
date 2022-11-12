#pragma once

#include "../../../shared/Networking/ProtocolUtils.hpp"
#include "../Components/Button.hpp"
#include "Menu.hpp"
#include <vector>

class Client;

class LobbySelectionMenu : public Menu {
public:
    LobbySelectionMenu(Client* client);

    void apply() override;

    void draw() override;

private:
    void queryLobbies();

    void refreshLobbies();

    std::string makeLobbyText(LobbyInfo lobby);

    Client* _client;

    std::vector<std::pair<LobbyInfo, Button>> _lobbies;

    float _buttonWidth;
    float _buttonHeight;
    float _buttonXPos;
    float _buttonYPos;
    Color _buttonColor;
};
