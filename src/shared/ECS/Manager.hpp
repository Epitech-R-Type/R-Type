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
#include <iterator>

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

        // Get component for given entity
        template<class T>
        T *getComponent(Id id) {
            Index i = getIndex(id);
            Index compId = getID<T>();

            // Make sure entity is valid
            if (0 > getIndex(this->_entities[i].id))
                return nullptr;
            
            // If entity doesn't have component return null
            if (!this->_entities[i].components[compId])
                return nullptr;

            return this->_compPools[compId]->getComp(i);
        }

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

        // Set components to exclude in view
        template <class ...Excluded>
        void setExcluded() {
            int excludedIds[] = {getID<Excluded...>()};
            this->_excludedInView.reset();

            for (auto excluded : excludedIds)
                this->_excludedInView[excluded] = 1;
        }

        // Reset components to exclude in view
        void resetExcluded() { this->_excludedInView.reset(); }

        // ITERATOR IMPLEMENTATION
        template <class ...Comps>
        class Iterator {
            public:
                Iterator(Index start, Manager *man): _currIndex(start), _man(man) {
                    int includedIds[] = {getID<Comps...>()};

                    // Check if no args passed, get entities with all comps                    
                    if (!sizeof(includedIds)) {
                        this->_all = true;
                        return;
                    }
                    this->_all = false;

                    for (auto included : includedIds)
                        this->_wanted[included] = 1;
                };

                Id operator*() const {
                    return this->_man->_entities[_currIndex].id;
                };

                bool operator==(const Iterator<Comps...> &other) const {
                    return this->_currIndex == other._currIndex;
                };

                bool operator!=(const Iterator<Comps...> &other) const {
                    return this->_currIndex != other._currIndex;
                };

                Iterator<Comps...>& operator++() {
                    std::bitset wantedComps = ~this->_man->_excludedInView & this->_wanted;

                    do {
                        this->_currIndex++;
                    } while (this->_currIndex < this->_man->_entities.size()
                        && (this->_all ||
                            ((wantedComps & this->_man->_entities[this->_currIndex].components) == wantedComps)));
                };
                        
                private:
                    Index _currIndex;
                    Manager *_man;
                    std::bitset<MAX_COMPONENTS> _wanted;
                    bool _all;
        };

        template <class ...Comp>
        const Iterator<Comp...> begin() const
        {
            return Iterator(0, this);
        }

        template <class ...Comp>
        const Iterator<Comp...> end() const
        {
            return Iterator(this->_entities.size(), this);
        }

    private:
        std::vector<Entity> _entities;
        std::vector<Index> _unusedEntities;
        std::vector<std::unique_ptr<CompPool>> _compPools;

        std::bitset<MAX_COMPONENTS> _excludedInView;
};