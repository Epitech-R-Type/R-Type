
#include "HitboxSystem.hpp"
#include "../ECS/Components.hpp";
#include "../ECS/Manager.hpp";
#include "SpriteSystem.hpp"
#include <cmath>

HitboxSystem::HitboxSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void HitboxSystem::apply() {}

struct Point {
    float x;
    float y;
};

Point rotatePoint(Point point, Point origin, float rad) {

    Point rPoint = {
        x : (std::cos(rad) * point.x) + (std::sin(rad) * point.y) + origin.x,
        y : point.y = (std::cos(rad) * point.y) - (std::sin(rad) * point.x) + origin.y
    };

    return rPoint;
}

double toRad(int degree) {
    constexpr double pi = 22 / 7;
    return degree * (pi / 180.0);
}

inline double width() {
    (Animation::Sheets[animation->animationID].frameWidth * animation->scale)
}

inline double height() {
    (Animation::Sheets[animation->animationID].frameHeight * animation->scale)
}

void HitboxSystem::hitboxFromAnimation(EntityID entity) {
    const Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(entity);
    const Position::Component* position = this->_ECS->getComponent<Position::Component>(entity);

    const double radRotation = toRad(animation->rotation);
    const Point origin {
        position->xPos;
        position->yPos;
    }

    const Point tLeft = rotatePoint({position->xPos, position->yPos}, origin, radRotation);
    const Point tRight = rotatePoint({position->xPos, position->yPos + width()}, origin, radRotation);
    const Point bLeft = rotatePoint({position->xPos + height(), position->yPos}, origin, radRotation);
    const Point bRight = rotatePoint({position->xPos + height(), position->yPos + width()}, origin, radRotation);

    // this->_ECS->addComponent<Hitbox::Component>(entity, {})
}