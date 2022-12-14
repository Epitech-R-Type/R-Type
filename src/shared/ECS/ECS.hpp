/*
** EPITECH PROJECT, 2022
** ECS.hpp
** File description:
** .
*/

#pragma once

#include <chrono>

// CONSTANTS
#define MAX_ENTITIES 4194304
#define MAX_COMPONENTS 32

#define INVALID_INDEX -1

#define WINDOW_WIDTH 1600.0
#define WINDOW_HEIGHT 900.0

// TYPE DEFINITIONS

// Type used to represent entity ID
// Using "long long" as it sits in 8B of memory
// Allowing us to use 4B for the entity index
// and 4B for the entity version
typedef long long EntityID;

// Readability typedefs
// Each of these sit in 4B of memory
typedef int Index;
typedef int Version;

// GLOBALS
extern Index g_idCounter; // Used by getID function

// FUNCTIONS

// This function returns a unique ID for each type specified in template
// parameter
// NOTE: Do not forget to initialize global in main
template <typename T>
Index getID() {
    static Index outputID = g_idCounter++;
    return outputID;
}

// Extracts Index from ID from top 4B
inline Index getIndex(EntityID id) {
    return (id >> 32);
}

// Extracts Version from ID from bottom 4B
inline Version getVersion(EntityID id) {
    return (Version)id;
}

inline bool isValid(EntityID id) {
    return !(0 > getIndex(id));
}
// Create Id from Index and Version
inline EntityID createId(Index i, Version v) {
    EntityID new_id = (EntityID)i; // Put index in new_id
    new_id = new_id << 32;         // Shift index to top bytes
    new_id = new_id | (EntityID)v; // Add version to bottom bytes

    return new_id;
}

inline std::chrono::time_point<std::chrono::system_clock> getNow() {
    return std::chrono::system_clock::now();
};

class ECSManager;
