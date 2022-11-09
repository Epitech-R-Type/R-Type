/*
** EPITECH PROJECT, 2022
** Utilities.hpp
** File description:
** .
*/

#pragma once

#ifndef __FUNCTION_NAME__
#ifdef WIN32 // WINDOWS
#define __FUNCTION_NAME__ __FUNCTION__
#define NC ""
#define REDL ""
#else //*NIX
#define __FUNCTION_NAME__ __func__
#define NC "\e[0m"
#define REDL "\e[0;31m"
#endif
#endif

#define ERRORLOG(msg) std::cout << REDL << "[ERROR]   " << __FUNCTION_NAME__ << ": " << msg << NC << std::endl
#define WARNING(msg) std::cout << "[WARNING] " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define LOG(msg) std::cout << "[LOG]     " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define DEBUG(msg) // std::cout << "[DEBUG]   " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define HAS_KEY(map, key) (map.find(key) != map.end())

#include <asio.hpp>
#include <string>
#include <vector>
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> timePoint;

enum Move {
    UP = 1,
    DOWN = -1,
    LEFT = 3,
    RIGHT = -3,
};

enum SFXID {
    INVALID = -1,
    KNOCK = 0,
    LIGHT_GUNSHOT = 1,
    HEAVY_GUNSHOT = 2,
};

enum SongID {
    BOSS,
    NORMAL,
};

struct Point {
    double x;
    double y;
};

namespace Utilities {
    /**
     * splits a string
     * */
    std::vector<std::string> splitStr(std::string str, std::string sep);

    /**
     * rotate a point around an origin
     * */
    Point rotate(Point point, Point origin, double rad);

    /**
     * trasnform degree to radians
     * */
    double toRadians(double degree);

    /**
     * Generate fixed order of comppools, better alternative would be using a map instead
     * */
    void createCompPoolIndexes();

    bool isPortAvailable(int port);
}; // namespace Utilities
