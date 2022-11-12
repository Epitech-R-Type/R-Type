/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Level
*/

#ifndef LEVEL_HPP_
#define LEVEL_HPP_

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

class Level { // put into vector to use all levels
public:
    Level() = default;
    ~Level() = default;
    void createLvl();

private:
    std::vector<int> _backGrounds = {11};
    int _enemys = 3;
    int _songs = 2;
    int _boss = 1;
    std::fstream createFile();
    int countLvls();
    std::string getBackground();
    std::string getMusic();
    std::string getMin();
    std::string getMax(int min);
    std::string getInterval();
    std::string getEnemy();
    std::string getBoss();
    int getWaves();
};

#endif /* !LEVEL_HPP_ */
