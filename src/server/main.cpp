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

    // // em.deleteEntity(ent1);

    // em.addComp<Health>(ent1, {5});
    em.addComp<Armor>(ent1, {6});
    // em.addComp<Position>(ent1, {6, 5});

    // if (!comp2)
    //     std::cout << "comp is null" << std::endl;

    // // std::cout << "comp2: " << comp2->test << std::endl;

    // std::cout << "Get TestComp1" << std::endl;

    // for (auto beg = em.begin<armor, health>(); beg != em.end<armor, health>(); ++beg) {

    //     if (em.getComponent<armor>(*beg) == nullptr) {
    //         continue;
    //     }

    //     std::cout << "zguegue" << em.getComponent<armor>(*beg)->test << getIndex(*beg) << std::endl;
    // }

    // std::cout << "Get TestComp2" << std::endl;

    // for (auto beg = em.begin<testComp2>(); beg != em.end<testComp2>(); ++beg) {

    //     if (em.getComponent<testComp2>(*beg) == nullptr) {
    //         continue;
    //     }

    //     std::cout << "zguegue" << em.getComponent<testComp2>(*beg)->test << getIndex(*beg) << std::endl;
    // }

    std::cout << entityToString<Armor>(ent1, &em) << std::endl;

    std::string str = entityToString<Armor>(ent1, &em);
    stringToEntity(str, &em);

    std::cout << entityToString<Armor>(ent1, &em) << std::endl;
}
