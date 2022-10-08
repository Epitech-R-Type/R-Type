/*
** EPITECH PROJECT, 2022
** Manager.cpp
** File description:
** .
*/

#include "Manager.hpp"

Id Manager::newEntity() {
    // If previously free'd entities available use those in preference
    if (!this->_unusedEntities.empty()) {
        // Pop last index from unusedEntities vector
        Index i = this->_unusedEntities.back();
        this->_unusedEntities.pop_back();

        // Increment previous entities version for use in createId
        Version updatedVersion = getVersion(this->_entities[i].id) + 1;

        // Create new id and push to entities
        Id id = createId(i, updatedVersion);
        this->_entities[i].id = id;

        return id;
    } else {
        // Create new id and entity
        Index i = this->_entities.size();
        Version v = 0;
        Id id = createId(i, v);

        Entity ent = Entity {id, 0};

        // Push to entities
        this->_entities.push_back(ent);

        return id;
    }
}

void Manager::deleteEntity(Id id)
{
    Index i = getIndex(id);

    // Check entity is valid
    if (0 > i)
        return;

    // Create Id with invalid index
    Id deletedEntity = createId(-1, getVersion(id));
    
    // Reset Entity
    this->_entities[i].components.reset();
    this->_entities[i].id = deletedEntity;

    // Append to unused entities for reuse in the futur
    this->_unusedEntities.push_back(i);
}

