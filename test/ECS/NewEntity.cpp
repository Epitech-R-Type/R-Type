/*
** EPITECH PROJECT, 2022
** NewEntity.cpp
** File description:
** .
*/

#include "EcsFixture.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test creation of single entity
TEST_F(EcsFixture, SingleEntity) {
    EXPECT_GT(_man->newEntity(), -1);
}

// Test creation of multiple entities
TEST_F(EcsFixture, MaxEntities) {
    // Instantiate max ent entities
    for (int i = 0; i < MAX_ENTITIES; i++) {
        EntityID newEnt = _man->newEntity();
        EXPECT_GT(newEnt, -1);
        EXPECT_TRUE(_man->entityExists(newEnt));
    }
}

// Tests creations of MAX_ENTITIES + 1
TEST_F(EcsFixture, TooManyEntities) {
    // Instantiate max ent entities
    for (int i = 0; i < MAX_ENTITIES; i++)
        _man->newEntity();
    EXPECT_EQ(_man->newEntity(), -1);
}

// TEST_F(EcsFixture, )
