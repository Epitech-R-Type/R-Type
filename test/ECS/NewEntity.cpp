/*
** EPITECH PROJECT, 2022
** NewEntity.cpp
** File description:
** .
*/

#include "EcsFixture.hpp"
#include <gtest/gtest.h>
#include <vector>

// ─── New Entity Tests ────────────────────────────────────────────────────────

// Test creation of single entity
TEST_F(EcsFixture, NewSingleEntity) {
    EXPECT_GT(_man->newEntity(), -1);
}

// Test creation of multiple entities
TEST_F(EcsFixture, NewMaxEntities) {
    // Instantiate max ent entities
    for (int i = 0; i < MAX_ENTITIES; i++) {
        EntityID newEnt = _man->newEntity();
        EXPECT_GT(newEnt, -1);
        EXPECT_TRUE(_man->entityExists(newEnt));
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

    _man->deleteEntity(id);
    id = _man->newEntity();

    EXPECT_TRUE(_man->entityExists(id));
}

// ─── Delete Entity Tests ─────────────────────────────────────────────────────

// Test deletion of single entity
TEST_F(EcsFixture, DelSingleEntity) {
    EntityID id = _man->newEntity();

    _man->deleteEntity(id);
    // EXPECT_TRUE(_man->entityIsActive(id));
    EXPECT_FALSE(_man->entityExists(id));
}

// Test double deletion of entity
TEST_F(EcsFixture, DoubleDelSingleEntity) {
    EntityID id = _man->newEntity();

    _man->deleteEntity(id);
    _man->deleteEntity(id);
    // EXPECT_TRUE(_man->entityIsActive(id));
    EXPECT_FALSE(_man->entityExists(id));
}
