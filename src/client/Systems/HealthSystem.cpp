#include "HealthSystem.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "SpriteSystem.hpp"
#include "Systems.hpp"
#include "raylib.h"

HealthSystem::HealthSystem(ECSManager* ECS) {
    this->_ECS = ECS;
}

void HealthSystem::setPlayer(EntityID playerID) {
    this->_player = playerID;
};

void HealthSystem::drawPlayerHP() {
    if (!isValid(this->_player))
        return;
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

        const float centerX = position->x + (float)Animation::Sheets[anim->animationID].frameWidth * anim->scale / 2.0;
        const float centerY = position->y + (float)Animation::Sheets[anim->animationID].frameHeight * anim->scale / 2.0;

        Point center = Utilities::rotate({centerX, centerY}, {position->x, position->y}, Utilities::toRadians(anim->rotation));

        if (health->visible) {
            float width = Animation::Sheets[anim->animationID].frameWidth * anim->scale;
            float height = Animation::Sheets[anim->animationID].frameWidth * anim->scale;

            const float remainingHP = width * ((float)health->health / (float)health->maxHealth);
            DrawRectangle(center.x - width / 2, center.y - height * 1.5, width, barHeight, WHITE);
            DrawRectangle(center.x + 1 - width / 2, center.y + 1 - height * 1.5, remainingHP, barHeight - 2, RED);
        }
    }

    this->drawPlayerHP();
}