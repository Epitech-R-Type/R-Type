#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "../shared/systems/UUID.hpp"
#include "./systems/Systems.hpp"
#include "Client.hpp"
#include "raylib.h"

#include <iostream>

Index g_idCounter = 0;

int main() {
    Client client;

    client.mainLoop();
}
