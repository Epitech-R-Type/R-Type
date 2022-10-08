#include <iostream>
#include <asio.hpp>

#include "Server.hpp"
#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"

Index g_idCounter = 0;

struct testComp {
    int test;
};

int main() {
    std::cout << "Server: Hello, World!" << std::endl;
    
    std::cout << "Here" << std::endl;
    Manager em;

    Id ent1 = em.newEntity();
    Id ent2 = em.newEntity();

    em.deleteEntity(ent1);

    testComp *comp = em.addComp<testComp>(ent1, {5});
    testComp *comp2 = em.addComp<testComp>(ent2, {6});


    if (!comp)
        std::cout << "comp is null" << std::endl;

    std::cout << "comp2: " << comp2->test << std::endl;

    return 0;
}