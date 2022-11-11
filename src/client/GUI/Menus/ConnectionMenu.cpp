#include "ConnectionMenu.hpp"
#include "../../Client.hpp"

ConnectionMenu::ConnectionMenu(Client* client) {
    this->_client = client;
    this->_init();
}

void ConnectionMenu::apply() {
    if (!this->_ipPrompt->getDone() || this->_done)
        return;

    std::vector<std::string> connectionData = Utilities::splitStr(this->_ipPrompt->getInput(), ":");
    std::cout << connectionData[0] << " " << connectionData[1] << std::endl;
    if (connectionData.size() != 2)
        return this->_init();

    const std::string ip = connectionData[0];
    int udpPort = std::stol(connectionData[1]);

    this->_client->connect(ip, udpPort);

    this->_done = true;
}

void ConnectionMenu::draw() {
    BeginDrawing();

    ClearBackground(BLACK);

    this->_ipPrompt->draw();

    EndDrawing();
}

void ConnectionMenu::_init() {
    this->_done = false;
    this->_ipPrompt = std::make_unique<TextBox>(std::string("127.0.0.1:3501"), CENTER(20 * 30, 60), 20 * 30, 60, 20, 30);
}

bool ConnectionMenu::getDone() {
    return this->_done;
}