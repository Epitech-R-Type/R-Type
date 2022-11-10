
#include "Server.hpp"

// Instantiation of global id counter
// TOUCH THIS AND EVERYTHING BREAKS
Index g_idCounter = 0;

int main() {
    Server serv;

    serv.setup();
    serv.mainLoop();
}
