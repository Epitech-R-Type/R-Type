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
#else //*NIX
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define REDL "\e[0;31m"
#define ERROR(msg) std::cout << REDL << "[ERROR]   " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define WARNING(msg) std::cout << "[WARNING] " << __FUNCTION_NAME__ << ": " << msg << std::endl
#define LOG(msg) std::cout << "[LOG]     " << __FUNCTION_NAME__ << ": " << msg << std::endl

#include <string>
#include <vector>

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
};
