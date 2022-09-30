#pragma once

#include "ECS.hpp"

class PositionComponent : public Component
{
    private:
        int xpos;
        int ypos;

    public:
        // PositionComponent();
        int x() {return xpos;}
        int y() {return ypos;}
        void init() override {
            xpos = 3;
            ypos = 3;
        }
        void setPos(int x, int y) {
            xpos = x;
            ypos = y;
        }
};