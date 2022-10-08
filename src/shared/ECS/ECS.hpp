/*
** EPITECH PROJECT, 2022
** ECS.hpp
** File description:
** .
*/

#pragma once

// CONSTANTS
#define MAX_ENTITIES 1024
#define MAX_COMPONENTS 4

#define INVALID_INDEX -1

// TYPE DEFINITIONS

// Type used to represent entity ID
// Using "long long" as it sits in 8B of memory
// Allowing us to use 4B for the entity index
// and 4B for the entity version
typedef long long Id;

// Readability typedefs
// Each of these sit in 4B of memory
typedef long Index;
typedef long Version;

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
inline Index getIndex(Id id) {
    return (id >> 32);
}

// Extracts Version from ID from bottom 4B
inline Version getVersion(Id id) {
    return (Version)id;
}

// Create Id from Index and Version
inline Id createId(Index i, Version v) {
    Id new_id = i;          // Put index in new_id
    new_id = new_id << 32;  // Shift index to top bytes
    new_id = new_id | v;    // Add version to bottom bytes

    return new_id;
}
