#include "../WindowsGuard.hpp"

#include "Client.hpp"

Index g_idCounter = 0;

int main() {
    InitWindow(1600, 900, "R-Type");

    Client client;

    client.setup();
    client.mainLoop();

    return 0;
}
