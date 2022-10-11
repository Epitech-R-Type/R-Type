#include <asio.hpp>
#include <iostream>

#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "Server.hpp"

// Instantiation of global id counter
// TOUCH THIS AND EVERYTHING BREAKS
Index g_idCounter = 0;

int main() {
    Server serv;

    serv.setup();
}
