#include "../../src/shared/ECS/Components.hpp"
#include "../../src/shared/ECS/ECSManager.hpp"
#include <gtest/gtest.h>

TEST(Armor, ToString) {
    Armor::Component armor{
        armor : 0,
    };

    std::string strArmor = Armor::toString(armor);

    EXPECT_STREQ("0;", strArmor.c_str());
}

TEST(Armor, ToEntity) {
    auto manager = std::make_shared<ECSManager>();

    EntityID id = manager->newEntity();

    manager->addComp<Armor::Component>(id, {armor : 2});

    EXPECT_EQ(manager->getComponent<Armor::Component>(id)->armor, 2);

    std::vector<std::string> args{std::to_string(id), "0;"};

    Armor::applyUpdate(args, id, manager);

    EXPECT_EQ(manager->getComponent<Armor::Component>(id)->armor, 0);
}

TEST(Health, ToString) {
    Health::Component health{
        health : 32,
        maxHealth : 32,
        visible : false,
    };

    std::string str = Health::toString(health);

    EXPECT_STREQ("32,32,0;", str.c_str());
}

TEST(Health, ToEntity) {
    auto manager = std::make_shared<ECSManager>();

    EntityID id = manager->newEntity();

    manager->addComp<Health::Component>(id, {
        health : 32,
        maxHealth : 32,
        visible : false,
    });

    EXPECT_EQ(manager->getComponent<Health::Component>(id)->health, 32);
    EXPECT_EQ(manager->getComponent<Health::Component>(id)->maxHealth, 32);
    EXPECT_EQ(manager->getComponent<Health::Component>(id)->visible, false);

    std::vector<std::string> args{std::to_string(id), "12,12,1;"};

    Health::applyUpdate(args, id, manager);

    EXPECT_EQ(manager->getComponent<Health::Component>(id)->health, 12);
    EXPECT_EQ(manager->getComponent<Health::Component>(id)->maxHealth, 12);
    EXPECT_EQ(manager->getComponent<Health::Component>(id)->visible, true);
}
