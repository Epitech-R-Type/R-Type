/*
** EPITECH PROJECT, 2022
** EcsFixture.cpp
** File description:
** .
*/

#include "EcsFixture.hpp"

EcsFixture::EcsFixture() {
    // Instantiate entity manager
    this->_man = std::make_shared<ECSManager>();
}
