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

// Test entity creation with given id
TEST_F(EcsFixture, NewEntityWithID) {
    EntityID sourceID = createId(25, 35);

    EntityID id = _man->newEntity(sourceID);

    // Check valid entity
    EXPECT_GT(id, -1);
    EXPECT_TRUE(_man->isValidEntity(id));

    // Check correct info
    EXPECT_EQ(getVersion(id), 35);
    EXPECT_EQ(getIndex(id), 25);
}

// Test given id creation with assign id creation
// Also test more than maxEnt
TEST_F(EcsFixture, NewEntityBothWays) {
    _man->newEntity(createId(1, 2));
    ASSERT_GT(_man->newEntity(), -1);

    EntityID id1 = _man->newEntity(createId(345, 1));
    ASSERT_GT(id1, -1);
    ASSERT_EQ(getVersion(id1), 1);
    ASSERT_EQ(getIndex(id1), 345);

    // Test assignation on same spot
    id1 = _man->newEntity(createId(345, 1));
    ASSERT_GT(id1, -1);
    ASSERT_EQ(getVersion(id1), 1);
    ASSERT_EQ(getIndex(id1), 345);

    for (int i = 0; i < MAX_ENTITIES - 4; i++)
        _man->newEntity();

    EntityID id2 = _man->newEntity(createId(2, 5));
    ASSERT_EQ(id2, -1);
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

// ─── Component Interaction Tests ─────────────────────────────────────────────────────────────────

// Test hasComponent Invalid entity
TEST_F(EcsFixture, HasComponentInvalidEntity) {
    EXPECT_FALSE(_man->hasComponent<int>(1234234));
}

// Test hasComponent true
TEST_F(EcsFixture, HasComponentTrue) {
    EntityID id = _man->newEntity();

    _man->addComp<int>(id, 5);
    EXPECT_TRUE(_man->hasComponent<int>(id));
}

// Test hasComponent false
TEST_F(EcsFixture, HasComponentFalse) {
    EntityID id = _man->newEntity();

    EXPECT_FALSE(_man->hasComponent<int>(id));
}

// Test getSetComponents
TEST_F(EcsFixture, GetSetComponents) {
    EntityID id1 = _man->newEntity();
    EntityID id2 = _man->newEntity();

    std::bitset<MAX_COMPONENTS> empty;
    std::bitset<MAX_COMPONENTS> notEmpty;
    notEmpty[getID<int>()] = 1;

    _man->addComp<int>(id2, 1);

    ASSERT_EQ(empty, _man->getSetComponents(id1));
    ASSERT_EQ(notEmpty, _man->getSetComponents(id2));
    ASSERT_EQ(empty, _man->getSetComponents(-1));
}

// Test removeComponent on valid entity and comp
TEST_F(EcsFixture, RemoveValidComponent) {
    EntityID id = _man->newEntity();

    _man->addComp<int>(id, 1);
    _man->removeComp<int>(id);

    ASSERT_FALSE(_man->hasComponent<int>(id));
}

// Test remove component on incalid component
TEST_F(EcsFixture, RemoveInvalidComponent) {
    EntityID id = _man->newEntity();

    // Test for no crash
    _man->removeComp<int>(id);
    ASSERT_FALSE(_man->hasComponent<int>(id)); // dunno why
}

TEST_F(EcsFixture, RemoveComponentInvalidEntity) {
    EntityID id = _man->newEntity();
    _man->deleteEntity(id);

    _man->removeComp<int>(id);
}

// Test adding single component
TEST_F(EcsFixture, AddSingleComponent) {
    EntityID id = _man->newEntity();

    int* ptr = _man->addComp<int>(id, 5);
    EXPECT_TRUE(_man->hasComponent<int>(id));
    EXPECT_TRUE(ptr);

    EXPECT_EQ(*ptr, 5);
}

// ─── Ecsmanager Utility Tests ────────────────────────────────────────────────────────────────────

TEST_F(EcsFixture, FlushEcs) {
    EntityID id1 = _man->newEntity();
    EntityID id2 = _man->newEntity();

    _man->deleteEntity(id2);

    _man->addComp<int>(id1, 5);
    _man->addComp<float>(id1, 5.5);

    _man->pushModified(id1);
    _man->pushModified(id2);

    for (int i = 0; i < MAX_ENTITIES - 2; i++)
        _man->newEntity();

    _man->flush();

    EXPECT_EQ(0, _man->getAllModified().size());
    EXPECT_EQ(0, _man->getUnusedEntities().size());
    EXPECT_EQ(0, _man->getEntities().size());
    EXPECT_EQ(0, _man->getCompPools().size());
}
