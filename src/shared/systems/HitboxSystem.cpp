
#include "HitboxSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"
#include "raylib.h"
#include <cmath>
#include <complex>
#include <iostream>

#define DRAW_HITBOX

HitboxSystem::HitboxSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void HitboxSystem::apply() {
    for (auto beg = this->_ECS->begin<Hitbox::Component, Position::Component>(); beg != this->_ECS->end<Hitbox::Component, Position::Component>();
         ++beg) {

        Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(*beg);
        Position::Component* position = this->_ECS->getComponent<Position::Component>(*beg);

        Hitbox::Component* hitbox = this->updateHitbox(*beg);

#ifdef DRAW_HITBOX
        DrawLine(hitbox->topLeft.x, hitbox->topLeft.y, hitbox->topRight.x, hitbox->topRight.y, RED);
        DrawLine(hitbox->topLeft.x, hitbox->topLeft.y, hitbox->botLeft.x, hitbox->botLeft.y, RED);
        DrawLine(hitbox->botLeft.x, hitbox->botLeft.y, hitbox->botRight.x, hitbox->botRight.y, RED);
        DrawLine(hitbox->botRight.x, hitbox->botRight.y, hitbox->topRight.x, hitbox->topRight.y, RED);
#endif

        DrawCircle(position->xPos, position->yPos, 2, BLUE);
        HitboxSystem::checkCollision(*beg);
    }
}

bool HitboxSystem::isColliding(EntityID entity1, EntityID entity2) {
    const Hitbox::Component* collisionBox1 = this->_ECS->getComponent<Hitbox::Component>(entity1);
    const Hitbox::Component* collisionBox2 = this->_ECS->getComponent<Hitbox::Component>(entity2);

    if (                                                         //
        collisionBox1->topRight.x > collisionBox2->topLeft.x     //
        && collisionBox1->topRight.x < collisionBox2->topRight.x //
        && collisionBox1->topRight.y > collisionBox2->topLeft.y  //
        && collisionBox1->topRight.y < collisionBox2->botLeft.y  //
    ) {
        return true;
    }

    if (                                                         //
        collisionBox1->botRight.x > collisionBox2->topLeft.x     //
        && collisionBox1->botRight.x < collisionBox2->topRight.x //
        && collisionBox1->botRight.y > collisionBox2->topLeft.y  //
        && collisionBox1->botRight.y < collisionBox2->botLeft.y  //
    ) {
        return true;
    }

    if (                                                        //
        collisionBox1->botLeft.x > collisionBox2->topRight.x    //
        && collisionBox1->botLeft.x < collisionBox2->topLeft.x  //
        && collisionBox1->botLeft.y > collisionBox2->topRight.y //
        && collisionBox1->botLeft.y < collisionBox2->topRight.y //
    ) {
        return true;
    }

    if (                                                        //
        collisionBox1->topLeft.x > collisionBox2->topRight.x    //
        && collisionBox1->topLeft.x < collisionBox2->topLeft.x  //
        && collisionBox1->topLeft.y > collisionBox2->topRight.y //
        && collisionBox1->topLeft.y < collisionBox2->topRight.y //
    ) {
        return true;
    }

    return false;
}

/**
 * returns who entity collided with
 * */
void HitboxSystem::checkCollision(EntityID entity) {
    for (auto beg = this->_ECS->begin<Hitbox::Component>(); beg != this->_ECS->end<Hitbox::Component>(); ++beg) {
        if (*this->_ECS->getComponent<Team::Component>(entity) == *this->_ECS->getComponent<Team::Component>(*beg))
            continue;

        if (!isColliding(entity, *beg))
            continue;

        std::cout << "is colliding" << std::endl;
        if (this->_ECS->hasComponent<Health::Component>(*beg) && this->_ECS->hasComponent<Damage::Component>(entity))
            this->_ECS->getComponent<Health::Component>(*beg)->health -= this->_ECS->getComponent<Damage::Component>(entity)->damage;
        if (this->_ECS->hasComponent<Health::Component>(entity) && this->_ECS->hasComponent<Damage::Component>(*beg))
            this->_ECS->getComponent<Health::Component>(entity)->health -= this->_ECS->getComponent<Damage::Component>(*beg)->damage;
    }
}

Point rotate(Point point, Point origin, double rad) {
    std::complex<double> P = {point.x, point.y};
    std::complex<double> Q = {origin.x, origin.y};

    std::complex<double> res = (P - Q) * std::polar(1.0, rad) + Q;
    return {
        res.real(),
        res.imag(),
    };
}

double toRad(double degree) {
    constexpr double pi = 22 / 7;
    return (double)degree * (pi / 180.0);
}

Hitbox::Component HitboxSystem::buildHitbox(Animation::Component* animation, Position::Component* position) {
    const double radRotation = toRad(animation->rotation);
    const double xPos = position->xPos;
    const double yPos = position->yPos;

    const Point origin{xPos, yPos};

    const double width = Hitbox::getWidth(animation);
    const double height = Hitbox::getHeight(animation);

    Point tLeft = rotate({xPos, yPos}, origin, radRotation);
    Point tRight = rotate({xPos + width, yPos}, origin, radRotation);
    Point bLeft = rotate({xPos, yPos + height}, origin, radRotation);
    Point bRight = rotate({xPos + width, yPos + height}, origin, radRotation);

    return {tLeft, tRight, bLeft, bRight};
}

Hitbox::Component* HitboxSystem::updateHitbox(EntityID entity) {
    Animation::Component* animation = this->_ECS->getComponent<Animation::Component>(entity);
    Position::Component* position = this->_ECS->getComponent<Position::Component>(entity);

    const auto newHitbox = HitboxSystem::buildHitbox(animation, position);

    Hitbox::Component* hitbox = this->_ECS->getComponent<Hitbox::Component>(entity);

    hitbox->topRight.x = newHitbox.topRight.x;
    hitbox->topRight.y = newHitbox.topRight.y;

    hitbox->topLeft.x = newHitbox.topLeft.x;
    hitbox->topLeft.y = newHitbox.topLeft.y;

    hitbox->botRight.x = newHitbox.botRight.x;
    hitbox->botRight.y = newHitbox.botRight.y;

    hitbox->botLeft.x = newHitbox.botLeft.x;
    hitbox->botLeft.y = newHitbox.botLeft.y;

    return hitbox;
};
