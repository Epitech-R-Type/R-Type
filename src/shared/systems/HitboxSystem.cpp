
#include "HitboxSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"
#include <cmath>

HitboxSystem::HitboxSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void HitboxSystem::apply() {}

bool HitboxSystem::isColliding(EntityID entity1, EntityID entity2) {
    const Hitbox::Component* collisionBox1 = this->_ECS->getComponent<Hitbox::Component>(entity1);
    const Hitbox::Component* collisionBox2 = this->_ECS->getComponent<Hitbox::Component>(entity2);

    if (collisionBox1->topRight.x > collisionBox2->topRight.x && //
        collisionBox1->topRight.x < collisionBox2->topLeft.x &&  //
        collisionBox1->topRight.y > collisionBox2->topRight.y && //
        collisionBox1->topRight.y < collisionBox2->botRight.y) {
        return true;
    }

    if (collisionBox1->topLeft.x > collisionBox2->topRight.x && //
        collisionBox1->topLeft.x < collisionBox2->topLeft.x &&  //
        collisionBox1->topLeft.y > collisionBox2->topRight.y && //
        collisionBox1->topLeft.y < collisionBox2->botRight.y) {
        return true;
    }

    if (collisionBox1->botRight.x > collisionBox2->topRight.x && //
        collisionBox1->botRight.x < collisionBox2->topLeft.x &&  //
        collisionBox1->botRight.y > collisionBox2->topRight.y && //
        collisionBox1->botRight.y < collisionBox2->botRight.y) {
        return true;
    }

    if (collisionBox1->botLeft.x > collisionBox2->topRight.x && //
        collisionBox1->botLeft.x < collisionBox2->topLeft.x &&  //
        collisionBox1->botLeft.y > collisionBox2->topRight.y && //
        collisionBox1->botLeft.y < collisionBox2->botRight.y) {
        return true;
    }
}

/**
 * returns who entity collided with
 * */
EntityID HitboxSystem::checkCollision(EntityID entity) {
    for (auto beg = this->_ECS->begin<Hitbox::Component>(); beg != this->_ECS->end<Hitbox::Component>(); ++beg) {

        if (isColliding(entity, *beg)) {
            this->_ECS->deleteEntity(*beg);
            return *beg;
        }
    }
}

Point rotatePoint(Point point, Point origin, float rad) {

    Point rPoint = {
        x : (std::cos(rad) * point.x) + (std::sin(rad) * point.y) + origin.x,
        y : point.y = (std::cos(rad) * point.y) - (std::sin(rad) * point.x) + origin.y
    };

    return rPoint;
}

float toRad(int degree) {
    constexpr float pi = 22 / 7;
    return degree * (pi / 180.0);
}

inline float getWidth(const Animation::Component* animation) {
    return (Animation::Sheets[animation->animationID].frameWidth * animation->scale);
}

inline float getHeight(const Animation::Component* animation) {
    return (Animation::Sheets[animation->animationID].frameHeight * animation->scale);
}

void HitboxSystem::hitboxFromAnimation(EntityID entity) {
    const Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(entity);
    const Position::Component* position = this->_ECS->getComponent<Position::Component>(entity);

    const float radRotation = toRad(animation->rotation);
    const Point origin{x : position->xPos, y : position->yPos};

    const float height = getHeight(animation);

    const float width = getWidth(animation);

    Point tLeft = rotatePoint({position->xPos, position->yPos}, origin, radRotation);
    Point tRight = rotatePoint({position->xPos, position->yPos + width}, origin, radRotation);
    Point bLeft = rotatePoint({position->xPos + height, position->yPos}, origin, radRotation);
    Point bRight = rotatePoint({position->xPos + height, position->yPos + width}, origin, radRotation);

    this->_ECS->addComp<Hitbox::Component>(entity, {tLeft, tRight, bLeft, bRight});
}