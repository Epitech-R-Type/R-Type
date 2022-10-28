/*
** EPITECH PROJECT, 2022
** NewEntity.cpp
** File description:
** .
*/

#include "EcsFixture.hpp"
#include <gtest/gtest.h>
#include <vector>

// Init g_idCounter
Index g_idCounter = 0;

// ─── Ecs Utility Function Tests ──────────────────────────────────────────────────────────────────

TEST(EcsUtilities, IdUtilities) {
    EntityID id = createId(25, 98); // Arbitrary values

    EXPECT_EQ(getIndex(id), 25);
    EXPECT_EQ(getVersion(id), 98);
    EXPECT_TRUE(isValid(id));
}

TEST(EcsUtilities, GetId) {
    Index i1 = getID<int>();
    Index i2 = getID<long>();
    Index i3 = getID<int>();

    EXPECT_EQ(i1, 0);
    EXPECT_EQ(i2, 1);
    EXPECT_EQ(i3, 0);
}

// ─── New Entity Tests ────────────────────────────────────────────────────────────────────────────

// Test creation of single entity
TEST_F(EcsFixture, NewSingleEntity) {
    EntityID newEnt = _man->newEntity();

    EXPECT_GT(newEnt, -1);
    EXPECT_EQ(getVersion(newEnt), 0);
}

// Test creation of multiple entities
TEST_F(EcsFixture, NewMaxEntities) {
    // Instantiate max ent entities
    for (int i = 0; i < MAX_ENTITIES; i++) {
        EntityID newEnt = _man->newEntity();
        EXPECT_GT(newEnt, -1);
        ASSERT_EQ(getVersion(newEnt), 0);
        EXPECT_TRUE(_man->isValidEntity(newEnt));
    }
}

// Tests creations of MAX_ENTITIES + 1
TEST_F(EcsFixture, NewTooManyEntities) {
    // Instantiate max ent entities
    for (int i = 0; i < MAX_ENTITIES; i++)
        _man->newEntity();
    EXPECT_EQ(_man->newEntity(), -1);
}

// Test reuse of entity
TEST_F(EcsFixture, ReuseOldEntity) {
    EntityID id = _man->newEntity();
    Index oldIndex = getIndex(id);
    Version oldVersion = getVersion(id);

    _man->deleteEntity(id);
    id = _man->newEntity();

    Index newIndex = getIndex(id);
    Version newVersion = getVersion(id);

    EXPECT_EQ(oldIndex, newIndex);

    EXPECT_NE(oldVersion, newVersion);
    EXPECT_EQ(newVersion, 1);

    EXPECT_TRUE(_man->isValidEntity(id));
}

// ─── Delete Entity Tests ─────────────────────────────────────────────────────────────────────────

// Test deletion of single entity
TEST_F(EcsFixture, DelSingleEntity) {
    EntityID id = _man->newEntity();

    _man->deleteEntity(id);
    EXPECT_FALSE(_man->isValidEntity(id));
}

// Test double deletion of entity
TEST_F(EcsFixture, DoubleDelSingleEntity) {
    EntityID id = _man->newEntity();

    _man->deleteEntity(id);
    EXPECT_FALSE(_man->isValidEntity(id));
    _man->deleteEntity(id);
    EXPECT_FALSE(_man->isValidEntity(id));
}

// Test deletion of invalid entity number
TEST_F(EcsFixture, DelInvalidEntity) {
    _man->deleteEntity(12342345345);

    SUCCEED();
}
