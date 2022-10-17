//
// Created by mindoo on 11.05.22.
//

#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
    /**
     * Creates a timer, starting to count on creation
     * target is the timer length in seconds
     * */
    Timer(double target);

    /**
     * verifies if more time has passed than specified by target
     * */
    bool isExpired();

    /**
     * resets the timer time to now
     * */
    void resetTimer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    float _target;
    bool _expired = false;
};
