#include <iostream>
#include "../shared/ECS/ECS.hpp"

int main() {
    std::cout << "Client: Hello, World!" << std::endl;

    std::cout << getComponentID() << std::endl;
    std::cout << getComponentID() << std::endl;

    Entity x = Entity();

    std::cout << x.isActive() << std::endl;
    x.destroy();
    std::cout << x.isActive() << std::endl;
    return 0;
}
