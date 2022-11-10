#include "TextBox.hpp"

TextBox::TextBox(std::string defaultValue, float xPos, float yPos, float width, float height, int charCount) {
    this->_xPos = xPos;
    this->_yPos = yPos;
    this->_width = width;
    this->_height = height;
    this->rectangle = {xPos, yPos, width, height};
    this->_value = "";
    this->_defaultValue = defaultValue;
    this->_charCount = charCount;

    this->launchThread();
};

TextBox::TextBox(std::string defaultValue, float xPos, float yPos, float width, float height, int charCount, int fontSize) {
    this->_xPos = xPos;
    this->_yPos = yPos;
    this->_width = charCount * fontSize;
    this->_height = fontSize;
    this->rectangle = {xPos, yPos, width, height};
    this->_value = "";
    this->_defaultValue = defaultValue;
    this->_charCount = charCount;

    this->launchThread();
};

void TextBox::launchThread() {
    new std::thread([this](void) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            bool mouseOnText = false;

            if (CheckCollisionPointRec(GetMousePosition(), this->rectangle))
                mouseOnText = true;

            // Set the window's cursor to the I-Beam
            if (mouseOnText) {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = 0;
                while (0 < (key = GetCharPressed())) {
                    if ((key >= 32) && (key <= 125) && this->_value.size() < this->getCharCount()) {
                        std::lock_guard<std::mutex> lock(this->_mtx);
                        this->_value.push_back(key);
                    }
                }

                if (IsKeyDown(KEY_BACKSPACE) && !this->_value.empty()) {
                    this->_value.pop_back();
                }

                if (IsKeyPressed(KEY_ENTER))
                    break;

            } else
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        this->setDone(true);
    });
};

void TextBox::draw() {
    std::lock_guard<std::mutex> lock(this->_mtx);
    DrawRectangleRec(this->rectangle, WHITE);
    if (this->_value.size())
        DrawText(this->_value.c_str(), (int)this->_xPos + this->_width / 10, (int)this->_yPos + this->_height / 10, this->_fontSize, GRAY);
    else
        DrawText(this->_defaultValue.c_str(), (int)this->_xPos + this->_width / 10, (int)this->_yPos + this->_height / 10, this->_fontSize,
                 LIGHTGRAY);
};

std::string TextBox::getInput() {
    if (this->_value.size())
        return std::string(this->_value);
    else
        return std::string(this->_defaultValue);
}

bool TextBox::getDone() {
    return this->_done;
};

void TextBox::setDone(bool done) {
    this->_done = done;
}

int TextBox::getCharCount() {
    return this->_charCount;
}