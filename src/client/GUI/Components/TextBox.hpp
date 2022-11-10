#pragma once

#include "../../../shared/Utilities/ray.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#define MAX_INPUT_CHARS 255

class TextBox {
public:
    TextBox(std::string defaultValue, float xPos, float yPos, float width, float height, int charCount);

    TextBox(std::string defaultValue, float xPos, float yPos, float width, float height, int charCount, int fontSize);

    void draw();

    void launchThread();

    std::string getInput();

    bool getDone();

    void setDone(bool done);

    int getCharCount();

private:
    int _xPos = 0;
    int _yPos = 0;
    int _width = 0;
    int _height = 0;
    float _fontSize = 40;
    int _charCount = 40;
    bool _done;

    Rectangle rectangle;

    std::string _value;

    std::string _defaultValue;

    std::mutex _mtx;
};
