#include <iostream>
// #include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Components.hpp"

int main()
{
    std::cout << "Client: Hello, World!" << std::endl;

    Manager manager;
    auto &newPlayer(manager.addEntity());

    newPlayer.addComponent<Position>();

    std::cout << "x: " << newPlayer.getComponent<Position>().x() << ", y: " << newPlayer.getComponent<Position>().y() << std::endl;

    std::cout << newPlayer.isActive() << std::endl;
    newPlayer.destroy();
    std::cout << newPlayer.isActive() << std::endl;

    auto &zorg(manager.addEntity());

    newPlayer.addComponent<Health>();

    newPlayer.getComponent<Health>().setValues(10);

    zorg.addComponent<Health>();

    zorg.getComponent<Health>().setValues(10);

    zorg.addComponent<CollisionEffect>();

    newPlayer.addComponent<CollisionEffect>();

    newPlayer.getComponent<CollisionEffect>().setEffect(1, 0);

    zorg.getComponent<CollisionEffect>().setEffect(3, 0);

    std::cout << newPlayer.getComponent<Health>().getHealth() << zorg.getComponent<Health>().getHealth() << std::endl;

    zorg.getComponent<CollisionEffect>().collide(newPlayer);

    std::cout << newPlayer.getComponent<Health>().getHealth() << zorg.getComponent<Health>().getHealth() << std::endl;

    return 0;
}
