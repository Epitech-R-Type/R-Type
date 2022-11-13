#include "Button.hpp"
#include "../../../shared/Utilities/Utilities.hpp"

Button::Button()
    : Button("Button", 0, 0, 200, 50, WHITE) {}

Button::Button(std::string text, float x, float y, float width, float height, Color color) {
    this->_x = x;
    this->_y = y;
    this->_width = width;
    this->_height = height;
    this->_rec = Rectangle{x, y, width, height};
    this->_highlightRec = Rectangle{x + 3, y + 3, this->_width - 6, this->_height - 6};
    this->_color = color;
    this->_text = text;
    this->_fontSize = this->_width / text.size();
    this->_hovered = Interaction::normal;

    this->_textColor = Color{
        static_cast<unsigned char>(255 - this->_color.r),
        static_cast<unsigned char>(255 - this->_color.g),
        static_cast<unsigned char>(255 - this->_color.b),
        this->_color.a,
    };

    this->_hoveredColors = Color{
        static_cast<unsigned char>(this->_color.r - 40),
        static_cast<unsigned char>(this->_color.g - 40),
        static_cast<unsigned char>(this->_color.b - 40),
        this->_color.a,
    };

    this->_clickedColors = Color{
        static_cast<unsigned char>(this->_color.r - 80),
        static_cast<unsigned char>(this->_color.g - 80),
        static_cast<unsigned char>(this->_color.b - 80),
        this->_color.a,
    };
}

void Button::apply() {
    if (CheckCollisionPointRec(GetMousePosition(), this->_rec)) {
        this->_hovered = Interaction::hovered;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !this->_disabled) {
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

    DrawRectangleRec(this->_rec, this->_color);

    if (this->_hovered == Interaction::hovered)
        DrawRectangleRec(this->_highlightRec, this->_hoveredColors);
    else if (this->_hovered == Interaction::pressed)
        DrawRectangleRec(this->_highlightRec, this->_clickedColors);

    const float textWidth = this->_text.length() * (this->_fontSize / 2);

    const float textX = this->_x + (this->_width / 2) - (textWidth / 2);
    const float textY = this->_y + this->_height / 2 - this->_fontSize / 2;
    DrawText(this->_text.c_str(), textX, textY, this->_fontSize, this->_textColor);
}

bool Button::hasBeenPressed() {
    if (this->_hasBeenPressed) {
        this->_hasBeenPressed = false;
        return true;
    };
}

void Button::disable() {
    this->_disabled = true;
    this->_hasBeenPressed = false;
}