/*
** EPITECH PROJECT, 2022
** Utilities.hpp
** File description:
** .
*/

#pragma once

#include <asio.hpp>
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

    static bool isPortAvailable(int port);
};
