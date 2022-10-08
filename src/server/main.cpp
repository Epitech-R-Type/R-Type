#include <asio.hpp>
#include <iostream>

#include "../shared/ECS/ECS.hpp"
#include "../shared/ECS/Manager.hpp"
#include "Server.hpp"

Index g_idCounter = 0;

struct testComp {
    int test;
};

struct testComp2 {
    int test;
};

int main() {
    std::cout << "Server: Hello, World!" << std::endl;

    std::cout << "Here" << std::endl;
    Manager em;

    Id ent1 = em.newEntity();
    Id ent2 = em.newEntity();

    // em.deleteEntity(ent1);

    testComp* comp = em.addComp<testComp>(ent1, {5});
    testComp2* comp2 = em.addComp<testComp2>(ent2, {6});

    if (!comp2)
        std::cout << "comp is null" << std::endl;

    // std::cout << "comp2: " << comp2->test << std::endl;

    std::cout << "Get TestComp1" << std::endl;

    for (auto beg = em.begin<testComp>(); beg != em.end<testComp>(); ++beg) {
        
        if (em.getComponent<testComp>(*beg) == nullptr) {
            continue;
        }
        

        std::cout<< "zguegue" << em.getComponent<testComp>(*beg)->test << getIndex(*beg) << std::endl;
    }

    std::cout << "Get TestComp2" << std::endl;

    for (auto beg = em.begin<testComp2>(); beg != em.end<testComp2>(); ++beg) {
        
        if (em.getComponent<testComp2>(*beg) == nullptr) {
            continue;
        }
        
        std::cout<< "zguegue" << em.getComponent<testComp2>(*beg)->test << getIndex(*beg) << std::endl;
    }
}
