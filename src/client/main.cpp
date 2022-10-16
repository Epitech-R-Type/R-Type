#include "../WindowsGuard.hpp"

#include "Client.hpp"

Index g_idCounter = 0;

int main(int argc, char** argv) {
    InitWindow(1600, 900, "R-Type");

    Client client;

    if (argc > 0) {
        client.connect(argv[1]);
    }

    client.launchGame();

    return 0;
}
