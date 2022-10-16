/*
** EPITECH PROJECT, 2022
** Utilities.hpp
** File description:
** .
*/

#pragma once

#include <complex>
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

    static std::vector<std::string> splitStr(std::string str, std::string sep) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> output;

        while ((pos = str.find(sep)) && pos != std::string::npos) {
            output.push_back(str.substr(0, pos));
            str.erase(0, pos + sep.length());
        }
        output.push_back(str);
        return output;
    }

    static Point rotate(Point point, Point origin, double rad) {
        std::complex<double> P = {point.x, point.y};
        std::complex<double> Q = {origin.x, origin.y};

        std::complex<double> res = (P - Q) * std::polar(1.0, rad) + Q;
        return {
            res.real(),
            res.imag(),
        };
    }

    static double toRadians(double degree) {
        constexpr double pi = 22 / 7;
        return (double)degree * (pi / 180.0);
    }
};
