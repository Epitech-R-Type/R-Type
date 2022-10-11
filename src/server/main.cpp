#include <asio.hpp>
#include <iostream>

#include "../shared/ECS/Components.hpp"
#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "Server.hpp"

#include "../shared/systems/serialization.hpp"

Index g_idCounter = 0;

int main() {
    std::cout << "Server: Hello, World!" << std::endl;

    Manager em;

    EntityID ent1 = em.newEntity();
    EntityID ent2 = em.newEntity();

    em.addComp<Armor::Component>(ent1, {6});
    em.addComp<Health::Component>(ent1, {5});
    // em.addComp<Position>(ent1, {6.2, 5.4});

    std::cout << entityToString<Armor::Component, Health::Component, Position::Component>(ent1, &em) << std::endl;

    std::string str = entityToString<Armor::Component, Health::Component, Position::Component>(ent1, &em);

    stringToEntity(str, &em);
    em.getComponent<Health::Component>(ent1)->health = 9;
    em.getComponent<Armor::Component>(ent1)->armor = 2;
    // em.getComponent<Position>(ent1)->xPos = 2.2;

    std::cout << entityToString<Armor::Component, Health::Component, Position::Component>(ent1, &em) << std::endl;
}
