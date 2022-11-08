#include "TextBox.hpp"

TextBox::TextBox(std::string defaultValue, float xPos, float yPos, float width, float height) {
    this->rectangle = {xPos, yPos, width, height};
    this->_value = "";
};

void TextBox::getInput() {
    this->_inputGetter = std::thread([this](void) {
        while (true) {
            bool mouseOnText = false;

            if (CheckCollisionPointRec(GetMousePosition(), this->rectangle))
                mouseOnText = true;

            if (mouseOnText) {
                // Set the window's cursor to the I-Beam
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();

                // Check if more characters have been pressed on the same frame
                while (key > 0) {
                    // NOTE: Only allow keys in range [32..125]
                    if ((key >= 32) && (key <= 125) /*&& (this->_value.size() < MAX_INPUT_CHARS)*/) {
                        this->_value.push_back(key);
                        // letterCount++;
                    }

                    key = GetCharPressed(); // Check next character in the queue
                }
                std::cout << std::string(this->_value.begin(), this->_value.end()) << std::endl;
                ;

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (!this->_value.empty())
                        this->_value.pop_back();
                    // letterCount--;
                }
            } else
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    });
};

void TextBox::draw() {
    DrawRectangleRec(this->rectangle, WHITE);
};