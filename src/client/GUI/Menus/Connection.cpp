#include "Connection.hpp"

Connection::Connection(Client* client) {
    this->_client = client;
    this->_init();
}

void Connection::apply() {
    if (!this->_ipPrompt->getDone() || this->_done)
        return;

    std::vector<std::string> connectionData = Utilities::splitStr(this->_ipPrompt->getInput(), ":");
    std::cout << connectionData[0] << " " << connectionData[1] << std::endl;
    if (connectionData.size() != 2)
        return this->_init();

    const std::string ip = connectionData[0];
    int udpPort = std::stol(connectionData[1]);

    const bool connected = this->_client->connect(ip, udpPort);

    if (connected == false)
        return this->_init();
    this->_done = true;
}

void Connection::draw() {
    BeginDrawing();

    ClearBackground(BLACK);

    this->_ipPrompt->draw();

    EndDrawing();
}

void Connection::_init() {
    this->_done = false;
    this->_ipPrompt = std::make_unique<TextBox>(std::string("127.0.0.1:3501"), CENTER(20 * 30, 60), 20 * 30, 60, 20, 30);
}

bool Connection::getDone() {
    return this->_done;
}