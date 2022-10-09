/*
** EPITECH PROJECT, 2022
** Armor.hpp
** File description:
** .
*/

#pragma once

enum ComponentType { Armor_, Health_, Position_ };

struct Armor {
    int armor = 0;
};

struct Health {
    int health = 0;
};

struct Position {
    float xpos = 0;
    float ypos = 0;
};