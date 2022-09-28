#include <iostream>
#include "../shared/Components.hpp"

int main() {
    std::cout << "Client: Hello, World!" << std::endl;

    std::cout << getComponentID() << std::endl;
    std::cout << getComponentID() << std::endl;
    return 0;
}
