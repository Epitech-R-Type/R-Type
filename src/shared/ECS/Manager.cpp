/*
** EPITECH PROJECT, 2022
** Manager.cpp
** File description:
** .
*/

#include "Manager.hpp"

EntityID Manager::newEntity() {
    // If previously free'd entities available use those in preference
    if (!this->_unusedEntities.empty()) {
        // Pop last index from unusedEntities vector
        Index i = this->_unusedEntities.back();
        this->_unusedEntities.pop_back();

        // Increment previous entities version for use in createId
        Version updatedVersion = getVersion(this->_entities[i].id) + 1;

        // Create new id and push to entities
        EntityID id = createId(i, updatedVersion);
        this->_entities[i].id = id;

        return id;
    } else {
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

void Manager::deleteEntity(EntityID id) {
    Index i = getIndex(id);

    // Check entity is valid
    if (0 > getIndex(this->_entities[i].id))
        return;

    // Create Id with invalid index
    EntityID deletedEntity = createId(-1, getVersion(id));

    // Reset Entity
    this->_entities[i].components.reset();
    this->_entities[i].id = deletedEntity;

    // Append to unused entities for reuse in the futur
    this->_unusedEntities.push_back(i);
}
