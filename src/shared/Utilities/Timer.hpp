//
// Created by mindoo on 11.05.22.
//

#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
    Timer(float target);
    bool isExpired();
    void resetTimer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    float _target;
    bool _expired = false;
};
