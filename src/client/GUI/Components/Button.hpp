#pragma once

#include "../../../shared/Utilities/ray.hpp"
#include <string>

enum Interaction {
    normal,
    hovered,
    pressed,
};

class Button {
public:
    Button(std::string text, float x, float y, float width, float height, Color color);

    void apply();

    void draw();

    bool hasBeenPressed();

    void updateText(std::string text);

private:
    Rectangle _rec;
    Rectangle _highlightRec;

    Color _color;
    Color _textColor;
    Color _hoveredColors;
    Color _clickedColors;

    Interaction _hovered = Interaction::normal;
    bool _hasBeenPressed = false;
    float _x;
    float _y;
    float _width;
    float _height;
    int _fontSize;
    std::string _text;
};