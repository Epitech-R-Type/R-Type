/*
** EPITECH PROJECT, 2022
** CompPool.cpp
** File description:
** .
*/

#include "CompPool.hpp"

CompPool::CompPool(size_t compSize)
{
    this->_compSize = compSize;
    this->_data = new Byte[compSize * MAX_ENTITIES];
}

CompPool::~CompPool()
{
    delete[] this->_data;
}

void* CompPool::getComp(Index i)
{
    return (this->_data + i * this->_compSize);
}
