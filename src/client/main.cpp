#include <iostream>
// #include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Components.hpp"

int main() {
    std::cout << "Client: Hello, World!" << std::endl;

    std::cout << getComponentID() << std::endl;
    std::cout << getComponentID() << std::endl;

    Entity *x = new Entity();
    // x->addComponent<PositionComponent>();

    std::cout << x->isActive() << std::endl;
    x->destroy();
    std::cout << x->isActive() << std::endl;
    // std::cout << x.getComponent<PositionComponent>().x() << std::endl;
    // std::cout << x.getComponent<PositionComponent>().y() << std::endl;
    return 0;
}
