/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** main
*/

#include "Level.hpp"

int main()
{
    Level* newLevel = new Level;
    newLevel->createLvl();
    delete newLevel;
    return (0);
}