#include "Menu.hpp"

void Menu::setState(State state) {
    this->_state = state;
};
State Menu::getState() {
    return this->_state;
};