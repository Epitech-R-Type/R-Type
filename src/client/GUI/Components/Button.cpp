#include "Button.hpp"
#include "../../../shared/Utilities/Utilities.hpp"

inline constexpr unsigned char operator"" uc(unsigned long long arg) noexcept {
    return static_cast<unsigned char>(arg);
}

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

    this->_textColor = Color{
        r : static_cast<unsigned char>(255uc - this->_color.r),
        g : static_cast<unsigned char>(255uc - this->_color.g),
        b : static_cast<unsigned char>(255uc - this->_color.b),
        a : this->_color.a,
    };

    this->_hoveredColors = Color{
        r : static_cast<unsigned char>(this->_color.r - 40uc),
        g : static_cast<unsigned char>(this->_color.g - 40uc),
        b : static_cast<unsigned char>(this->_color.b - 40uc),
        a : this->_color.a,
    };

    this->_clickedColors = Color{
        r : static_cast<unsigned char>(this->_color.r - 80uc),
        g : static_cast<unsigned char>(this->_color.g - 80uc),
        b : static_cast<unsigned char>(this->_color.b - 80uc),
        a : this->_color.a,
    };
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

    DrawRectangleRec(this->_rec, this->_color);

    if (this->_hovered == Interaction::hovered)
        DrawRectangleRec(this->_highlightRec, this->_hoveredColors);
    else if (this->_hovered == Interaction::pressed)
        DrawRectangleRec(this->_highlightRec, this->_clickedColors);

    const float textWidth = this->_text.length() * (this->_fontSize / 2);
    DrawText(this->_text.c_str(), ALIGN_MIDDLE(textWidth), this->_y + this->_height / 10, this->_fontSize, this->_textColor);
}

bool Button::hasBeenPressed() {
    return this->_hasBeenPressed;
}