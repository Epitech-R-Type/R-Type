#pragma once

enum State {
    RUNNING,
    DONE,
    BACK

};

class Menu {
public:
    virtual void apply() = 0;

    virtual void draw() = 0;

    State getState();

    void setState(State);

protected:
    State _state = State::RUNNING;
};
