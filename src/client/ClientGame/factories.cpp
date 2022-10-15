#include "factories.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../systems/SpriteSystem.hpp"
#include "../systems/Systems.hpp"

EntityID makePlayer(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID player = ECS->newEntity();

    const float startX = (float)(GetScreenWidth() * (1.0 / 3.0));
    const float startY = (float)(GetScreenHeight() / 1.5);

    Position::Component* position = ECS->addComp<Position::Component>(player, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(player, {Animation::AnimationID::Vortex, 2, 45});
    ECS->addComp<Health::Component>(player, {25, 30});
    ECS->addComp<Player::Component>(player, {true});
    ECS->addComp<Armament::Component>(player, {Armament::ArmamentType::Bullet, 150, 50});
    ECS->addComp<Velocity::Component>(player, {10, 10});
    ECS->addComp<Hitbox::Component>(player, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(player, Team::Ally);

    return player;
}

void makeEndboss(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID endboss = ECS->newEntity();
    int players = 3;

    Animation::Component* animation = ECS->addComp<Animation::Component>(endboss, {Animation::AnimationID::Cluster, 1, -90.0});

    float xOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameHeight * 3;

    float yOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameWidth / 2;

    const float startX = (float)(GetScreenWidth() - xOffset);

    const float startY = (float)(GetScreenHeight() - yOffset);

    Position::Component* position = ECS->addComp<Position::Component>(endboss, {startX, startY});

    ECS->addComp<Health::Component>(endboss, {300 * players, 300 * players, true});

    ECS->addComp<Hitbox::Component>(endboss, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(endboss, Team::Enemy);
}

void makeEnemy(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID enemy = ECS->newEntity();

    const float startX = (float)(GetScreenWidth() / 2);

    const float startY = (float)(rand() % GetScreenHeight());

    Position::Component* position = ECS->addComp<Position::Component>(enemy, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(enemy, {Animation::AnimationID::Orb, 3});

    ECS->addComp<Velocity::Component>(enemy, {0, 0});
    ECS->addComp<Health::Component>(enemy, {10, 20, true});
    ECS->addComp<Damage::Component>(enemy, {20});

    ECS->addComp<Hitbox::Component>(enemy, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(enemy, Team::Enemy);
    ECS->addComp<Armament::Component>(enemy, {Armament::ArmamentType::Bullet, 150, 50});
}

EntityID getPlayerID(Manager* ECS) {
    for (auto beg = ECS->begin<Player::Component>(); beg != ECS->end<Player::Component>(); ++beg) {
        return *beg;
    }
    return -1;
}

void makeBullet(Manager* ECS, SpriteSystem* spriteSystem, EntityID source) {
    const EntityID bullet = ECS->newEntity();

    const Position::Component* sourcePos = ECS->getComponent<Position::Component>(source);
    const Velocity::Component* sourceVelocity = ECS->getComponent<Velocity::Component>(source);
    const Animation::Component* sourceAnimation = ECS->getComponent<Animation::Component>(source);
    const float centerX = sourcePos->xPos + (float)Animation::Sheets[sourceAnimation->animationID].animWidth / 2;
    const float centerY = sourcePos->yPos + (float)Animation::Sheets[sourceAnimation->animationID].animHeight / 2;

    Position::Component positionPre{};
    Velocity::Component velocity{};

    Team::Component team = *ECS->getComponent<Team::Component>(source);

    if (team == Team::Component::Ally) {
        positionPre.xPos = centerX - (float)Animation::Sheets[sourceAnimation->animationID].animWidth;
        positionPre.yPos = centerY;
        velocity.xVelocity = 40;
    }

    if (team == Team::Component::Enemy) {
        positionPre.xPos = centerX + (float)Animation::Sheets[sourceAnimation->animationID].animWidth;
        positionPre.yPos = centerY;
        velocity.xVelocity = -40;
    }

    Position::Component* position = ECS->addComp<Position::Component>(bullet, positionPre);
    ECS->addComp<Velocity::Component>(bullet, velocity);
    ECS->addComp<Health::Component>(bullet, {1});
    ECS->addComp<Damage::Component>(bullet, {10});
    Animation::Component* animation = ECS->addComp<Animation::Component>(bullet, {Animation::AnimationID::Laser, 1, 0, 1});
    ECS->addComp<Hitbox::Component>(bullet, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(bullet, *ECS->getComponent<Team::Component>(source));
}