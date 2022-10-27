/*
** EPITECH PROJECT, 2022
** EcsFixture.hpp
** File description:
** .
*/

#pragma once

#include <gtest/gtest.h>
#include <memory>

#include "../../src/shared/ECS/Manager.hpp"

class EcsFixture : public ::testing::Test {
public:
    EcsFixture();

protected:
    std::shared_ptr<ECSManager> _man;
};
