#pragma once

#include "../../../shared/Utilities/ray.hpp"
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#define MAX_INPUT_CHARS 255

class TextBox {
public:
    TextBox(std::string defaultValue, float xPos, float yPos, float width, float height, int charCount);

    ~TextBox();

    void draw();

    void launchThread();

    std::string getInput();

    bool getDone();

    void setDone(bool done);

    int getCharCount();

    std::atomic_bool isRunning();

private:
    int _xPos = 0;
    int _yPos = 0;
    int _width = 0;
    int _height = 0;
    float _fontSize = 40;
    int _charCount = 40;

    std::atomic_bool _done = false;

    Rectangle rectangle;

    std::string _value;

    std::string _defaultValue;

    std::mutex _mtx;
};
