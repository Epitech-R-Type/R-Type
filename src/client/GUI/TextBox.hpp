#pragma once

#include "../../shared/Utilities/ray.hpp"
#include <iostream>
#include <memory>
#include <thread>

#define MAX_INPUT_CHARS 255

class TextBox {
public:
    TextBox(std::string defaultValue, float xPos, float yPos, float width, float height);

    void draw();

    void getInput();

private:
    void pushKeyStroke() {}

    Rectangle rectangle;

    std::string _value;

    std::thread _inputGetter;
};
