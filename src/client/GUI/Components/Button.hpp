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
    Button();

    Button(std::string text, float x, float y, float width, float height, Color color);

    /**
     * Verifies interactions and detects clicks
     */
    void apply();

    /**
     * Draws butten according to params
     * and interaction state
     */
    void draw();

    /**
     * returns true if button has been pressed,
     * but only once, it resets that state after each call
     */
    bool hasBeenPressed();

    /**
     * update the displayed text
     */
    void updateText(std::string text);

    /**
     * deactivates click detections and pressed state
     */
    void disable();

private:
    Rectangle _rec;
    Rectangle _highlightRec;

    Color _color;
    Color _textColor;
    Color _hoveredColors;
    Color _clickedColors;

    Interaction _hovered = Interaction::normal;
    bool _hasBeenPressed = false;
    bool _disabled = false;
    float _x;
    float _y;
    float _width;
    float _height;
    int _fontSize;
    std::string _text;
};