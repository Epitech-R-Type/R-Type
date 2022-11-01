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

#define ERROR(msg) std::cout << REDL << "[ERROR]   " << __FUNCTION_NAME__ << ": " << msg << NC << std::endl
#define WARNING(msg) std::cout << "[WARNING] " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define LOG(msg) std::cout << "[LOG]     " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define DEBUG(msg) // std::cout << "[DEBUG]   " << __FUNCTION_NAME__ << ": " << msg << std::endl

#include <asio.hpp>
#include <string>
#include <vector>

enum Move { UP = 1, DOWN = -1, LEFT = 3, RIGHT = -3 };

struct Point {
    double x;
    double y;
};
class Utilities {
public:
    Utilities();

    ~Utilities();

    /**
     * splits a string
     * */
    static std::vector<std::string> splitStr(std::string str, std::string sep);

    /**
     * rotate a point around an origin
     * */
    static Point rotate(Point point, Point origin, double rad);

    /**
     * trasnform degree to radians
     * */
    static double toRadians(double degree);

    /**
     * Generate fixed order of comppools, better alternative would be using a map instead
     * */
    static void createCompPoolIndexes();

    static bool isPortAvailable(int port);
};
