#include "Client.hpp"

Index g_idCounter = 0;

int main() {

    Client client;

    client.setup();
    client.launchGame();

    return 0;
}
