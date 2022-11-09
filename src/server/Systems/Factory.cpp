#include "Factory.hpp"
#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECSManager.hpp"
#include "../../shared/Systems/HitboxSystem.hpp"
#include "../../shared/Utilities/Utilities.hpp"

EntityID Factory::Ally::makePlayer(std::shared_ptr<ECSManager> ECS, int uniqueID) {
    EntityID player = ECS->newEntity();

    Position::Component* position = ECS->addComp<Position::Component>(player, {0, WINDOW_HEIGHT / 2});

    if (uniqueID == 0)
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipLightblue, 1, 0, 3});
    else if (uniqueID == 1)
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipPink, 1, 0, 3});
    else if (uniqueID == 2)
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipGreen, 1, 0, 3});
    else if (uniqueID == 3)
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipRed, 1, 0, 3});
    else if (uniqueID == 4)
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipDarkblue, 1, 0, 3});
    else
        ECS->addComp<Animation::Component>(player, {Animation::AnimationID::SpaceshipRGB, 1, 0, 3});

    Animation::Component* animation = ECS->getComponent<Animation::Component>(player);
    ECS->addComp<Health::Component>(player, {50, 50, true});
    ECS->addComp<Player::Component>(player, {true, uniqueID});
    ECS->addComp<Armament::Component>(player, {Armament::Type::Laser, 150, -1});
    ECS->addComp<Velocity::Component>(player, {10, 10});
    ECS->addComp<Hitbox::Component>(player, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(player, Team::Ally);
    ECS->addComp<ImmunityFrame::Component>(player, {1});
    ECS->addComp<Damage::Component>(player, {20});
    ECS->addComp<CollisionEffect::Component>(player, &CollisionEffect::dealDamage);

    ECS->pushModified(player);

    return player;
}

EntityID Factory::Enemy::makeEndboss(std::shared_ptr<ECSManager> ECS) {
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
    ECS->addComp<Armament::Component>(endboss, {Armament::Type::Buckshot, 1000, 50});
    ECS->addComp<CollisionEffect::Component>(endboss, &CollisionEffect::dealDamage);
    ECS->pushModified(endboss);
    return endboss;
}

EntityID Factory::Enemy::makeEnemy(std::shared_ptr<ECSManager> ECS, EnemyStats stats) {
    EntityID enemy = ECS->newEntity();

    const float startX = WINDOW_WIDTH;
    const float startY = rand() % (int)WINDOW_HEIGHT;

    Position::Component* position = ECS->addComp<Position::Component>(enemy, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(enemy, {stats.sprite, 3});

    ECS->addComp<Velocity::Component>(enemy, {-(stats.speed), 0});
    ECS->addComp<Health::Component>(enemy, {stats.health, stats.health, true});
    ECS->addComp<Damage::Component>(enemy, {stats.damage});

    ECS->addComp<Hitbox::Component>(enemy, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(enemy, Team::Enemy);
    ECS->addComp<Armament::Component>(enemy, {stats.armament, 1000, 50});
    ECS->addComp<CollisionEffect::Component>(enemy, &CollisionEffect::dealDamage);

    ECS->pushModified(enemy);
    return enemy;
}

EntityID bullet(std::shared_ptr<ECSManager> ECS, EntityID source, int velocityX, int velocityY, double rotation) {
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
        ECS->addComp<SoundCreation::Component>(bullet, {SFXID::HEAVY_GUNSHOT});
    }

    if (team == Team::Component::Enemy) {
        positionPre.x = center.x + (float)Animation::Sheets[sourceAnimation->animationID].animWidth;
        positionPre.y = center.y;
        velocity.x = -velocityX;
        velocity.y = -velocityY;
        ECS->addComp<SoundDestruction::Component>(bullet, {SFXID::LIGHT_GUNSHOT});
    }

    ECS->addComp<Velocity::Component>(bullet, velocity);
    ECS->addComp<Health::Component>(bullet, {1});
    ECS->addComp<Damage::Component>(bullet, {10});
    Animation::Component* animation = ECS->addComp<Animation::Component>(bullet, {Animation::AnimationID::Laser, 1, rotation, 1.5});
    Position::Component* position = ECS->addComp<Position::Component>(bullet, positionPre);
    ECS->addComp<Team::Component>(bullet, *ECS->getComponent<Team::Component>(source));
    ECS->addComp<Hitbox::Component>(bullet, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<CollisionEffect::Component>(bullet, &CollisionEffect::dealDamage);

    ECS->pushModified(bullet);
    return bullet;
}

void Factory::Weapon::makeLaser(std::shared_ptr<ECSManager> ECS, EntityID source) {
    bullet(ECS, source, 40, 0, 0);
}

void Factory::Weapon::makeBuckshot(std::shared_ptr<ECSManager> ECS, EntityID source) {
    bullet(ECS, source, 40, 0, 0);
    bullet(ECS, source, 40, 5, 6);
    bullet(ECS, source, 40, 10, 12);
    bullet(ECS, source, 40, -5, -6);
    bullet(ECS, source, 40, -10, -12);
}

void Factory::Misc::makeBackground(std::shared_ptr<ECSManager> ECS) {
    EntityID background = ECS->newEntity();

    ECS->addComp<Position::Component>(background, {0, 0});
    ECS->addComp<Animation::Component>(background, {Animation::AnimationID::Background, 0});
    ECS->addComp<Velocity::Component>(background, {2, 0});
    ECS->pushModified(background);
};
