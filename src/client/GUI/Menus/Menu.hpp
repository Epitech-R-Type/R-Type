#pragma once

class Menu {
public:
    virtual void apply() = 0;

    virtual void draw() = 0;

    virtual bool getDone() = 0;
};
