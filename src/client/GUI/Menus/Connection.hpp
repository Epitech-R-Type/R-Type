#pragma once

#include "../../Client.hpp"
#include "../Components/TextBox.hpp"

#define CENTER(width, height) ((GetScreenWidth() - width) / 2), ((GetScreenHeight() - height) / 2)

class Connection {
public:
    Connection(Client* client);

    void apply();

    void draw();

    bool getDone();

private:
    void _init();

    bool _done = false;

    Client* _client;

    std::unique_ptr<TextBox> _ipPrompt;
};
