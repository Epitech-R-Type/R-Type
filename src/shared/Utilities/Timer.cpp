//
// Created by mindoo on 11.05.22.
//

#include "Timer.hpp"

Timer::Timer(double target) {
    this->_target = target;
    this->_start = std::chrono::high_resolution_clock::now();
}

bool Timer::isExpired() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = currentTime - this->_start;
    if (duration.count() >= this->_target)
        this->_expired = true;
    return this->_expired;
}

void Timer::resetTimer() {
    this->_expired = false;
    this->_start = std::chrono::high_resolution_clock::now();
}