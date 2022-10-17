#include "Factory.hpp"
#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../../shared/Systems/HitboxSystem.hpp"
#include "../../shared/Utilities/Utilities.hpp"

EntityID Factory::Ally::makePlayer(ECSManager* ECS) {
    EntityID player = ECS->newEntity();

    Position::Component* position = ECS->addComp<Position::Component>(player, {0, 0});
    Animation::Component* animation = ECS->addComp<Animation::Component>(player, {Animation::AnimationID::Vortex, 2});
    ECS->addComp<Health::Component>(player, {50, 50});
    ECS->addComp<Player::Component>(player, {true});
    ECS->addComp<Armament::Component>(player, {Armament::Type::Buckshot, 150, -1});
    ECS->addComp<Velocity::Component>(player, {10, 10});
    ECS->addComp<Hitbox::Component>(player, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(player, Team::Ally);
    ECS->addComp<ImmunityFrame::Component>(player, {1});
    ECS->addComp<Damage::Component>(player, {20});
    ECS->addComp<CollisionEffect::Component>(player, &CollisionEffect::dealDamage);

    return player;
}

void Factory::Enemy::makeEndboss(ECSManager* ECS) {
    EntityID endboss = ECS->newEntity();
    int players = 0;
    for (auto beg = ECS->begin<Player::Component>(); beg != ECS->end<Player::Component>(); ++beg) {
        players++;
    }

    Animation::Component* animation = ECS->addComp<Animation::Component>(endboss, {Animation::AnimationID::Cluster, 1, -90.0});

    float xOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameHeight * 3;
    float yOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameWidth / 2;

    const float startX = WINDOW_WIDTH - xOffset;
    const float startY = WINDOW_HEIGHT - yOffset;

    Position::Component* position = ECS->addComp<Position::Component>(endboss, {startX, startY});

    ECS->addComp<Damage::Component>(endboss, {20});
    ECS->addComp<Health::Component>(endboss, {300 * players, 300 * players, false});
    ECS->addComp<Hitbox::Component>(endboss, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(endboss, Team::Enemy);
    ECS->addComp<CollisionEffect::Component>(endboss, &CollisionEffect::dealDamage);
}

void Factory::Enemy::makeEnemy(ECSManager* ECS) {
    EntityID enemy = ECS->newEntity();

    const float startX = WINDOW_WIDTH;
    const float startY = rand() % (int)WINDOW_HEIGHT;

    Position::Component* position = ECS->addComp<Position::Component>(enemy, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(enemy, {Animation::AnimationID::Orb, 3});

    ECS->addComp<Velocity::Component>(enemy, {-10, 0});
    ECS->addComp<Health::Component>(enemy, {20, 20, true});
    ECS->addComp<Damage::Component>(enemy, {20});

    ECS->addComp<Hitbox::Component>(enemy, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(enemy, Team::Enemy);
    ECS->addComp<Armament::Component>(enemy, {Armament::Type::Laser, 1000, 50});
    ECS->addComp<CollisionEffect::Component>(enemy, &CollisionEffect::dealDamage);
}

void bullet(ECSManager* ECS, EntityID source, int velocityX, int velocityY, double rotation) {
    const EntityID bullet = ECS->newEntity();

    const Position::Component* sourcePos = ECS->getComponent<Position::Component>(source);
    const Velocity::Component* sourceVelocity = ECS->getComponent<Velocity::Component>(source);
    const Animation::Component* sourceAnimation = ECS->getComponent<Animation::Component>(source);
    const Point oldCenter{
        sourcePos->x + (float)Animation::Sheets[sourceAnimation->animationID].frameWidth * sourceAnimation->scale / 2.0,
        sourcePos->y + (float)Animation::Sheets[sourceAnimation->animationID].frameHeight * sourceAnimation->scale / 2.0,
    };

    Point center = Utilities::rotate(oldCenter, {sourcePos->x, sourcePos->y}, Utilities::toRadians(sourceAnimation->rotation));

    Position::Component positionPre{};
    Velocity::Component velocity{};
    Team::Component team = *ECS->getComponent<Team::Component>(source);

    if (team == Team::Component::Ally) {
        positionPre.x = center.x - (float)Animation::Sheets[sourceAnimation->animationID].animWidth;
        positionPre.y = center.y;
        velocity.x = velocityX;
        velocity.y = velocityY;
    }

    if (team == Team::Component::Enemy) {
        positionPre.x = center.x + (float)Animation::Sheets[sourceAnimation->animationID].animWidth;
        positionPre.y = center.y;
        velocity.x = -velocityX;
        velocity.y = -velocityY;
    }

    ECS->addComp<Velocity::Component>(bullet, velocity);
    ECS->addComp<Health::Component>(bullet, {1});
    ECS->addComp<Damage::Component>(bullet, {1});
    Animation::Component* animation = ECS->addComp<Animation::Component>(bullet, {Animation::AnimationID::Laser, 1, rotation, 1.5});
    Position::Component* position = ECS->addComp<Position::Component>(bullet, positionPre);
    ECS->addComp<Team::Component>(bullet, *ECS->getComponent<Team::Component>(source));
    ECS->addComp<Hitbox::Component>(bullet, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<CollisionEffect::Component>(bullet, &CollisionEffect::dealDamage);
}

void Factory::Weapon::makeLaser(ECSManager* ECS, EntityID source) {
    bullet(ECS, source, 40, 0, 0);
}

void Factory::Weapon::makeBuckshot(ECSManager* ECS, EntityID source) {
    bullet(ECS, source, 40, 0, 0);
    bullet(ECS, source, 40, 5, 6);
    bullet(ECS, source, 40, 10, 12);
    bullet(ECS, source, 40, -5, -6);
    bullet(ECS, source, 40, -10, -12);
}