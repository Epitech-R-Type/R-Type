/*
** EPITECH PROJECT, 2022
** Manager.hpp
** File description:
** .
*/

#pragma once

#include "CompPool.hpp"
#include "ECS.hpp"

#include <bitset>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <vector>

struct Entity {
    EntityID id;
    std::bitset<MAX_COMPONENTS> components;
};

class ECSManager {
public:
    // Create entity
    EntityID newEntity();

    EntityID newEntity(EntityID id);

    // Delete entity
    void deleteEntity(EntityID id);

    bool entityIsActive(Index id);

    // Get component for given entity
    template <class T>
    T* getComponent(EntityID id) {
        Index i = getIndex(id);
        Index compId = getID<T>();

        // If entity doesn't have component return null
        if (!this->_entities[i].components[compId])
            return nullptr;

        return static_cast<T*>(this->_compPools[compId]->getComp(i));
    }

    template <class T>
    bool hasComponent(EntityID id) const {
        Index i = getIndex(id);
        Index compId = getID<T>();

        // Make sure entity is valid
        if (0 > getIndex(this->_entities[i].id))
            return false;

        // If entity doesn't have component return null
        if (!this->_entities[i].components[compId])
            return false;

        return true;
    }

    const std::bitset<MAX_COMPONENTS> getSetComponents(EntityID entity) const {
        Index i = getIndex(entity);
        // Make sure entity is valid
        if (0 > getIndex(this->_entities[i].id))
            return {};
        return this->_entities[i].components;
    };

    // Add comp
    template <class T>
    T* addComp(EntityID id, T comp) {
        Index i = getIndex(id);

        // Make sure entity is valid
        if (0 > getIndex(this->_entities[i].id))
            return nullptr;

        // Get unique id for component type
        Index compId = getID<T>();

        // Set bit of component in component bitset of entity
        this->_entities[i].components[compId] = 1;

        // Check pool if given component exists
        if (this->_compPools.size() <= compId) {
            // Create new pool
            this->_compPools.resize(compId + 1);
            this->_compPools[compId] = std::make_unique<CompPool>(sizeof(T));
        }

        // If unique_ptr is not initialized
        if (!this->_compPools[compId])
            this->_compPools[compId] = std::make_unique<CompPool>(sizeof(T));

        // Get our entities component memory
        T* component = (T*)this->_compPools[compId]->getComp(i);

        // Assign comp memory
        *component = comp;

        return (T*)component;
    }

    // Remove comp
    template <class T>
    void removeComp(EntityID id) {
        Index compIndex = getID<T>();
        Index i = getIndex(id);

        // Check against invalid entity
        if (0 > getIndex(this->_entities[i].id))
            return;

        // Reset corresponding bit in component bitset
        this->_entities[i].components[compIndex] = 0;
    }

    void removeComp(EntityID id, Index compId) {
        if (!this->isValidComp(id, compId))
            return;

        Index i = getIndex(id);

        this->_entities[i].components[compId] = 0;
    }

    // Set components to exclude in view
    template <class... Excluded>
    void setExcluded() {
        Index excludedIds[] = {getID<Excluded...>()};
        this->_excludedInView.reset();

        for (auto excluded : excludedIds)
            this->_excludedInView[excluded] = 1;
    }

    // Reset components to exclude in view
    void resetExcluded() {
        this->_excludedInView.reset();
    }

    bool isValidID(EntityID id) {
        Index i = getIndex(id);

        if (i == INVALID_INDEX || i >= this->_entities.size())
            return false;
        return true;
    }

    bool isValidComp(EntityID id, Index i) {
        if (!this->isValidID(id))
            return false;
        if (i >= MAX_COMPONENTS || i >= g_idCounter)
            return false;

        Index entityIndex = getIndex(id);
        if (!this->_entities[entityIndex].components[i])
            return false;
        return true;
    }

    bool entityExists(EntityID id) {
        Index i = getIndex(id);

        if (i >= this->_entities.size() || i >= MAX_ENTITIES)
            return false;
        if (getIndex(this->_entities[i].id) == INVALID_INDEX)
            return false;
        return true;
    }

    // ITERATOR IMPLEMENTATION
    template <class... Comps>
    class Iterator {
    public:
        Iterator(Index start, ECSManager* man)
            : _currIndex(start),
              _man(man) {
            Index includedIds[] = {getID<Comps>()...};
            this->_wanted.reset();

            for (auto included : includedIds)
                this->_wanted.set(included);
        };

        EntityID operator*() const {
            return this->_man->_entities[this->_currIndex].id;
        };

        bool operator==(const Iterator<Comps...>& other) {
            return this->_currIndex == other._currIndex;
        };

        bool operator!=(const Iterator<Comps...>& other) {
            return this->_currIndex != other._currIndex;
        };

        bool isValid() {
            std::bitset<MAX_COMPONENTS> wantedComps = ~this->_man->_excludedInView & this->_wanted;
            std::bitset<MAX_COMPONENTS> concombre = (this->_wanted & this->_man->_entities[this->_currIndex].components);

            return !this->_wanted.any() || this->_wanted == concombre && this->_man->isValidID(this->_man->_entities[this->_currIndex].id);
        }

        Iterator<Comps...>& operator++() {
            do {
                ++this->_currIndex;
            } while (this->_currIndex < this->_man->_entities.size() && !this->isValid());

            return *this;
        };

    private:
        Index _currIndex;
        ECSManager* _man;
        std::bitset<MAX_COMPONENTS> _wanted;
    };

    template <class... Comp>
    const Iterator<Comp...> begin() {
        auto iter = Iterator<Comp...>(0, this);

        // crashes otherwise
        if (this->_entities.size() == 0)
            return iter;

        // advance until first matching entity
        if (!iter.isValid())
            ++iter;
        return iter;
    }

    template <class... Comp>
    const Iterator<Comp...> end() {
        return Iterator<Comp...>(this->_entities.size(), this);
    }

    void flush();

    std::optional<EntityID> getModified();

    void pushModified(EntityID);

private:
    std::vector<EntityID> _modifiedEntities;

    std::vector<Entity> _entities;
    std::vector<Index> _unusedEntities;
    std::vector<std::unique_ptr<CompPool>> _compPools;

    std::bitset<MAX_COMPONENTS> _excludedInView;
};
