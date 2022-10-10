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

    std::cout << "Here" << std::endl;
    Manager em;

    EntityID ent1 = em.newEntity();
    EntityID ent2 = em.newEntity();

    em.addComp<Health>(ent1, {5});
    em.addComp<Armor>(ent1, {6});
    em.addComp<Position>(ent1, {6.2, 5.4});

    std::cout << entityToString<Armor, Health, Position>(ent1, &em) << std::endl;

    std::string str = entityToString<Armor, Health, Position>(ent1, &em);

    stringToEntity(str, &em);
    em.getComponent<Armor>(ent1)->armor = 2;
    em.getComponent<Health>(ent1)->health = 9;
    em.getComponent<Position>(ent1)->xpos = 2.2;

    std::cout << entityToString<Armor, Health, Position>(ent1, &em) << std::endl;
}
