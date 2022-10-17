
#include "HitboxSystem.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/Manager.hpp"
#include "../Utilities/Utilities.hpp"
#include <cmath>
#include <iostream>

HitboxSystem::HitboxSystem(std::shared_ptr<ECSManager> ECS) {
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

        DrawCircle(position->x, position->y, 2, BLUE);
#endif

        HitboxSystem::checkCollision(*beg);
    }
}

Point project(Point point, Point axis) {
    return Point{
        ((point.x * axis.x + point.y * axis.y) / (axis.x * axis.x + axis.y * axis.y)) * axis.x,
        ((point.x * axis.x + point.y * axis.y) / (axis.x * axis.x + axis.y * axis.y)) * axis.y,
    };
}

double toScalar(Point point, Point axis) {
    return point.x * axis.x + point.y * axis.y;
}

bool getOverlap(const Hitbox::Component* rectA, const Hitbox::Component* rectB, Point axis) {
    // p stands for projection
    // Rectangle A
    Point pTopRightA = project(rectA->topRight, axis);
    Point pTopLeftA = project(rectA->topLeft, axis);
    Point pBotRightA = project(rectA->topLeft, axis);
    Point pBotLeftA = project(rectA->botLeft, axis);

    // Rectangle B

    Point pTopRightB = project(rectB->topRight, axis);
    Point pTopLeftB = project(rectB->topLeft, axis);
    Point pBotRightB = project(rectB->topLeft, axis);
    Point pBotLeftB = project(rectB->botLeft, axis);

    std::vector<double> scalarValuesA;

    scalarValuesA.push_back(toScalar(pTopRightA, axis));
    scalarValuesA.push_back(toScalar(pTopLeftA, axis));
    scalarValuesA.push_back(toScalar(pBotRightA, axis));
    scalarValuesA.push_back(toScalar(pBotLeftA, axis));

    std::vector<double> scalarValuesB;

    scalarValuesB.push_back(toScalar(pTopRightB, axis));
    scalarValuesB.push_back(toScalar(pTopLeftB, axis));
    scalarValuesB.push_back(toScalar(pBotRightB, axis));
    scalarValuesB.push_back(toScalar(pBotLeftB, axis));

    double maxA = scalarValuesA[0];
    double minA = scalarValuesA[0];
    double maxB = scalarValuesB[0];
    double minB = scalarValuesB[0];

    for (int val : scalarValuesA) {
        if (val > maxA)
            maxA = val;
        if (val < minA)
            minA = val;
    }

    for (int val : scalarValuesB) {
        if (val > maxB)
            maxB = val;
        if (val < minB)
            minB = val;
    }

    return (minB <= maxA) && (maxB >= minA);
};

bool HitboxSystem::isColliding(EntityID entity1, EntityID entity2, std::shared_ptr<ECSManager> ECS) {
    const Hitbox::Component* rectA = ECS->getComponent<Hitbox::Component>(entity1);
    const Hitbox::Component* rectB = ECS->getComponent<Hitbox::Component>(entity2);

    Point Axis1 = {rectA->topRight.x - rectA->topLeft.x, rectA->topRight.y - rectA->topLeft.y};
    Point Axis2 = {rectA->topRight.x - rectA->botRight.x, rectA->topRight.y - rectA->botRight.y};
    Point Axis3 = {rectB->topLeft.x - rectB->botLeft.x, rectB->topLeft.y - rectB->botLeft.y};
    Point Axis4 = {rectB->topLeft.x - rectA->topRight.x, rectB->topLeft.y - rectA->topRight.y};

    return (getOverlap(rectA, rectB, Axis1) && getOverlap(rectA, rectB, Axis2) && getOverlap(rectA, rectB, Axis3) && getOverlap(rectA, rectB, Axis4));
}

bool HitboxSystem::isColliding(EntityID entity1, EntityID entity2) {
    return HitboxSystem::isColliding(entity1, entity2, this->_ECS);
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

        (*this->_ECS->getComponent<CollisionEffect::Component>(entity))(entity, *beg, this->_ECS);
        (*this->_ECS->getComponent<CollisionEffect::Component>(*beg))(*beg, entity, this->_ECS);
    }
}

Hitbox::Component HitboxSystem::buildHitbox(Animation::Component* animation, Position::Component* position) {
    const double radRotation = Utilities::toRadians(animation->rotation);
    const double xPos = position->x;
    const double yPos = position->y;

    const Point origin{xPos, yPos};

    const double width = Animation::Sheets[animation->animationID].frameWidth * animation->scale;
    const double height = (Animation::Sheets[animation->animationID].frameHeight * animation->scale);

    Point tLeft = Utilities::rotate({xPos, yPos}, origin, radRotation);
    Point tRight = Utilities::rotate({xPos + width, yPos}, origin, radRotation);
    Point bLeft = Utilities::rotate({xPos, yPos + height}, origin, radRotation);
    Point bRight = Utilities::rotate({xPos + width, yPos + height}, origin, radRotation);

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
