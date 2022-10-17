/*
** EPITECH PROJECT, 2022
** Manager.cpp
** File description:
** .
*/

#include "Manager.hpp"
#include <algorithm>

EntityID ECSManager::newEntity() {
    // If previously free'd entities available use those in preference
    if (!this->_unusedEntities.empty() && false) { // Pop last index from unusedEntities vector
        Index i = this->_unusedEntities.back();
        this->_unusedEntities.pop_back();

        // Increment previous entities version for use in createId
        Version updatedVersion = getVersion(this->_entities[i].id) + 1;

        // Create new id and push to entities
        EntityID id = createId(i, updatedVersion);
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
    Entity ent = Entity{getIndex(entityID), getVersion(entityID)};

    // Push to entities
    this->_entities.push_back(ent);

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

bool ECSManager::entityIsActive(EntityID id) {
    return (std::find(this->_unusedEntities.begin(), this->_unusedEntities.end(), id) == this->_unusedEntities.end());
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