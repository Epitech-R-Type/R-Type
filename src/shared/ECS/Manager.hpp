/*
** EPITECH PROJECT, 2022
** Manager.hpp
** File description:
** .
*/

#pragma once

#include <vector>
#include <bitset>
#include <memory>
#include <iostream>

#include "ECS.hpp"
#include "CompPool.hpp"

struct Entity {
  Id id;
  std::bitset<MAX_COMPONENTS> components;
};

class Manager {
  public:
    // Create entity
    Id newEntity();

    // Delete entity
    void deleteEntity(Id id);

    // Add comp
    template<class T>
    T *addComp(Id id, T comp) {
      Index i = getIndex(id);

      // Make sure entity is valid
      if (0 > getIndex(this->_entities[i].id))
        return nullptr;

      // Get unique id for component type
      Index compId = getID<T>();

      // Set bit of component in component bitset of entity
      this->_entities[i].components[compId] = 1;

      // Check if pool for given component exists
      if (this->_compPools.size() <= compId) {
        // Create new pool
        this->_compPools.resize(compId + 1);
        this->_compPools[compId] = std::make_unique<CompPool>(sizeof(T));
      }

      // If unique_ptr is not initialized
      if (!this->_compPools[compId])
        this->_compPools[compId] = std::make_unique<CompPool>(sizeof(T));

      // Get our entities component memory
      T *component = (T*)this->_compPools[compId]->getComp(i);

      // Assign comp memory
      *component = comp;

      return (T*)component;
    }

    // Remove comp
    template <class T>
    void removeComp(Id id) {
      Index compIndex = getID<T>();
      Index i = getIndex(id);

      // Check against invalid entity
      if (0 > getIndex(this->_entities[i].id))
        return;

      // Reset corresponding bit in component bitset
      this->_entities[i].components[compIndex] = 0;
    }

  private:
    std::vector<Entity> _entities;
    std::vector<Index> _unusedEntities;
    std::vector<std::unique_ptr<CompPool>> _compPools;
};