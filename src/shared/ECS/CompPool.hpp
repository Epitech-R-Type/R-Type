/*
** EPITECH PROJECT, 2022
** CompPool.hpp
** File description:
** CompPool
*/

#pragma once

#include <cstddef>
#include <vector>

#include "ECS.hpp"

typedef char Byte;

// Note: We could maybe optimize this by implementing a sparse array
class CompPool {
public:
    CompPool(size_t compSize);
    ~CompPool();

    // Get comp at specific index
    void* getComp(Index i);

private:
    Byte* _data;
    size_t _compSize;
};
