/*
** EPITECH PROJECT, 2022
** Manager.cpp
** File description:
** .
*/

#include "ECSManager.hpp"
#include "../Utilities/Utilities.hpp"
#include <algorithm>

// ─── Entity Creation And Deletion ────────────────────────────────────────────────────────────────

EntityID ECSManager::newEntity() {
    // If previously free'd entities available use those in preference
    if (!this->_unusedEntities.empty()) { // Pop last index from unusedEntities vector
        Index i = this->_unusedEntities.back();
        this->_unusedEntities.pop_back();

        // Increment previous entities version for use in createId
        Version v = getVersion(this->_entities[i].id);

        // Create new id and push to entities
        EntityID id = createId(i, v);
        this->_entities[i].id = id;
        return id;
    } else {
        if (this->_entities.size() >= MAX_ENTITIES)
            return -1;
        // Create new id and entity
        Index i = this->_entities.size();
        Version v = 0;
        EntityID id = createId(i, v);

        Entity ent = Entity{id, 0};

        // Push to entities
        this->_entities.push_back(ent);

        return id;
    }
}

EntityID ECSManager::newEntity(EntityID entityID) {
    if (this->_entities.size() >= MAX_ENTITIES)
        return -1;
    // Create new id and entity

    if (this->_entities.size() < getIndex(entityID))
        this->_entities.resize(getIndex(entityID) + 1);

    // Push to entities
    this->_entities.emplace(this->_entities.begin() + getIndex(entityID), Entity{entityID, 0});

    return entityID;
}

void ECSManager::deleteEntity(EntityID id) {
    Index i = getIndex(id);

    // Check entity is valid
    if (0 > getIndex(this->_entities[i].id))
        return;

    // Create Id with invalid index
    EntityID deletedEntity = createId(-1, getVersion(id) + 1);

    // Reset Entity
    this->_entities[i].components.reset();
    this->_entities[i].id = deletedEntity;

    // Append to unused entities for reuse in the futur
    this->_unusedEntities.push_back(i);
}

// ─── Component Interaction Methods ───────────────────────────────────────────────────────────────

const std::bitset<MAX_COMPONENTS> ECSManager::getSetComponents(EntityID entity) const {
    Index i = getIndex(entity);
    // Make sure entity is valid
    if (0 > getIndex(this->_entities[i].id))
        return {};
    return this->_entities[i].components;
}

void ECSManager::removeComp(EntityID id, Index compId) {
    if (!this->entityHasComp(id, compId))
        return;

    Index i = getIndex(id);

    this->_entities[i].components[compId] = 0;
}

// ─── Utility Methods ─────────────────────────────────────────────────────────────────────────────

bool ECSManager::entityIsActive(Index id) {
    return (std::find(this->_unusedEntities.begin(), this->_unusedEntities.end(), id) == this->_unusedEntities.end());
}

bool ECSManager::entityHasComp(EntityID id, Index i) {
    if (!this->isValidEntity(id))
        return false;
    if (i >= MAX_COMPONENTS || i >= g_idCounter)
        return false;

    Index entityIndex = getIndex(id);
    if (!this->_entities[entityIndex].components[i])
        return false;
    return true;
}

bool ECSManager::isValidEntity(EntityID id) {
    Index i = getIndex(id);
    Version v = getVersion(id);

    if (i >= this->_entities.size() || i >= MAX_ENTITIES || i == INVALID_INDEX)
        return false;
    if (getIndex(this->_entities[i].id) == INVALID_INDEX || getVersion(this->_entities[i].id) != v)
        return false;
    return true;
}

void ECSManager::flush() {
    for (auto entity : this->_entities) {
        this->deleteEntity(entity.id);
    }

    this->_entities = {};
    this->_unusedEntities = {};

    this->_compPools = std::vector<std::unique_ptr<CompPool>>{};
    this->_excludedInView.reset();
};

// ─── Modified Entities Handling ──────────────────────────────────────────────────────────────────

std::optional<EntityID> ECSManager::getModified() {
    if (this->_modifiedEntities.empty())
        return {};
    std::optional<EntityID> modified = this->_modifiedEntities.back();
    this->_modifiedEntities.pop_back();
    return modified;
};

void ECSManager::pushModified(EntityID entityID) {
    if (std::find(this->_modifiedEntities.begin(), this->_modifiedEntities.end(), entityID) != this->_modifiedEntities.end())
        return;
    this->_modifiedEntities.push_back(entityID);
};

// ─── Iterator Implementation ─────────────────────────────────────────────────────────────────────

void ECSManager::resetExcluded() {
    this->_excludedInView.reset();
}
