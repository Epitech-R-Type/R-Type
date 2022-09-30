#include <iostream>
// #include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Components.hpp"

int main() {
    std::cout << "Client: Hello, World!" << std::endl;

    std::cout << getComponentID() << std::endl;
    std::cout << getComponentID() << std::endl;

    Manager manager;
    auto& newPlayer(manager.addEntity());
    newPlayer.addComponent<PositionComponent>();

    std::cout << newPlayer.getComponent<PositionComponent>().x() << std::endl;
    std::cout << newPlayer.getComponent<PositionComponent>().y() << std::endl;
    std::cout << newPlayer.isActive() << std::endl;
    newPlayer.destroy();
    std::cout << newPlayer.isActive() << std::endl;
    return 0;
}
