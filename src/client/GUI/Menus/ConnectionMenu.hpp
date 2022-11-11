#pragma once

#include "../Components/TextBox.hpp"
#include "Menu.hpp"

class Client;

#define CENTER(width, height) ((GetScreenWidth() - width) / 2), ((GetScreenHeight() - height) / 2)

class ConnectionMenu : public Menu {
public:
    ConnectionMenu(Client* client);
    ~ConnectionMenu(){

    };

    void apply() override;

    void draw() override;

    bool getDone() override;

private:
    void _init();

    bool _done = false;

    Client* _client;

    std::unique_ptr<TextBox> _ipPrompt;
};
