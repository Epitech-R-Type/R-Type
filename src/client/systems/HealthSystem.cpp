#include "HealthSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "SpriteSystem.hpp"
#include "raylib.h"

HealthSystem::HealthSystem(Manager* ECS) {
    this->_ECS = ECS;
}

void HealthSystem::setPlayer(EntityID playerID) {
    this->_player = playerID;
};

void HealthSystem::drawPlayerHP() {
    Health::Component* health = this->_ECS->getComponent<Health::Component>(this->_player);
    const float width = 150;

    const float barHeight = 20;
    const float remainingHP = width * ((float)health->health / (float)health->maxHealth);

    DrawRectangle(GetScreenWidth() * (1.0 / 20.0), GetScreenHeight() - barHeight, width, barHeight, WHITE);
    DrawRectangle(GetScreenWidth() * (1.0 / 20.0) + 1, GetScreenHeight() - barHeight + 1, remainingHP, barHeight - 2, RED);
};

void HealthSystem::apply() {
    constexpr int barHeight = 8;

    for (auto beg = this->_ECS->begin<Health::Component, Position::Component>(); beg != this->_ECS->end<Health::Component, Position::Component>();
         ++beg) {

        EntityID id = *beg;

        Health::Component* health = this->_ECS->getComponent<Health::Component>(id);
        Position::Component* position = this->_ECS->getComponent<Position::Component>(id);

        Animation::Component* anim = this->_ECS->getComponent<Animation::Component>(id);

        if (health->visible) {
            float width = SpriteSystem::ANIMATION_SHEET[anim->animationID].frameWidth * anim->scale;
            float height = SpriteSystem::ANIMATION_SHEET[anim->animationID].frameWidth * anim->scale;

            const float remainingHP = width * ((float)health->health / (float)health->maxHealth);
            DrawRectangle(position->xPos - width / 2, position->yPos - height * 1.5, width, barHeight, WHITE);
            DrawRectangle(position->xPos + 1 - width / 2, position->yPos + 1 - height * 1.5, remainingHP, barHeight - 2, RED);
        }
    }

    this->drawPlayerHP();
}