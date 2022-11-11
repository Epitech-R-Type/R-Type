#include "Button.hpp"
#include "../../../shared/Utilities/Utilities.hpp"

Button::Button(std::string text, float x, float y, float width, float height, Color color) {
    this->_x = x;
    this->_y = y;
    this->_width = width;
    this->_height = height;
    this->_rec = Rectangle{x, y, width, height};
    this->_highlightRec = Rectangle{x + 3, y + 3, this->_width - 6, this->_height - 6};
    this->_color = color;
    this->_text = text;
    this->_fontSize = this->_height * 0.8;
    this->_hovered = Interaction::normal;
}

void Button::apply() {
    if (CheckCollisionPointRec(GetMousePosition(), this->_rec)) {
        this->_hovered = Interaction::hovered;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            this->_hovered = Interaction::pressed;
            this->_hasBeenPressed = true;
        }
    } else {
        this->_hovered = Interaction::normal;
    }
}

void Button::updateText(std::string text) {
    this->_text = text;
};

void Button::draw() {
    Color flipped = Color{255 - this->_color.r, 255 - this->_color.g, 255 - this->_color.b, this->_color.a};
    Color hoveredColors = Color{this->_color.r - 40, this->_color.g - 40, this->_color.b - 40, this->_color.a};
    Color clickedColors = Color{this->_color.r - 80, this->_color.g - 80, this->_color.b - 80, this->_color.a};

    DrawRectangleRec(this->_rec, this->_color);
    if (this->_hovered == Interaction::hovered)
        DrawRectangleRec(this->_highlightRec, hoveredColors);
    else if (this->_hovered == Interaction::pressed)
        DrawRectangleRec(this->_highlightRec, clickedColors);

    DrawText(this->_text.c_str(), ALIGN_MIDDLE(this->_text.length() * (this->_fontSize / 2)), this->_y + this->_height / 10, this->_fontSize,
             flipped);
}

bool Button::hasBeenPressed() {
    return this->_hasBeenPressed;
}