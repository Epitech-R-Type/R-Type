/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** Level
*/

#ifndef LEVEL_HPP_
#define LEVEL_HPP_

#include <chrono>
#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <fstream>
#include <filesystem>

typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> timePoint;

inline auto GetNow() {
    return std::chrono::time_point_cast<timePoint::duration>(std::chrono::system_clock::time_point(std::chrono::system_clock::now()));
};

// class decribing an enemy wave of a level
class Wave {
    public:
        bool finishedFlag;
        bool endless;
        int minSpawned;
        int maxSpawned;
        float spawnInterval;
        int spawned;
        int typesFlag; //use powers of 2 to describe what enemys are contained in the wave to make binary checks later (type1 = 1, type2 = 2, type3 = 4, ...)
    private:
};

class LevelClock {
    public:
        std::chrono::duration<double> boss() {const auto _now = GetNow(); return (_now - _boss);};
        std::chrono::duration<double> interval() {const auto _now = GetNow(); return (_now - _enemy);};
        void start() {this->_enemy = GetNow(); _boss = GetNow();};
        timePoint _enemy;
        timePoint _boss;
    private:
};

class Level { // put into vector to use all levels
    public:
        Level() = default;
        ~Level() = default;
        void loadLvlNb(int nb);
        bool nextLvl();
        void startLvl();
        void exec();
        void createLvl();
        void print();
        LevelClock clock;

    private:
        void writeLvlToFile(std::fstream &lvlFile, Level &toWrite);
        std::fstream createFile();
        int countLvls();
        void clear();
        std::vector<Wave> levelWaves;
        int waveNb;
        int bossCountdown;
        int mapSpeed;
        int mapId;
        int currentLvl;
};

#endif /* !LEVEL_HPP_ */
