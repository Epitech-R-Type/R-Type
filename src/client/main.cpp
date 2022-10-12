#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "./systems/Systems.hpp"
#include "Client.hpp"
#include "raylib.h"
#include "uuid.h"

#include <iostream>
#include <regex>

Index g_idCounter = 0;

int main() {
    Client client;

    client.mainLoop();
}
