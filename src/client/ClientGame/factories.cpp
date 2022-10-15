#include "factories.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../systems/SpriteSystem.hpp"
#include "../systems/Systems.hpp"

void dealDamage(EntityID attacker, EntityID defender, ECSManager* ECS) {
    if (!ECS->hasComponent<Health::Component>(defender) || !ECS->getComponent<Damage::Component>(attacker))
        return;
    Health::Component* healthC = ECS->getComponent<Health::Component>(defender);
    ImmunityFrame::Component* immunityFrame = ECS->getComponent<ImmunityFrame::Component>(defender);
    Armor::Component* armor = ECS->getComponent<Armor::Component>(defender);
    Damage::Component* damageC = ECS->getComponent<Damage::Component>(attacker);

    if (immunityFrame != nullptr) {
        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - immunityFrame->timer;

        if (elapsed_seconds.count() < immunityFrame->duration)
            return;
        immunityFrame->timer = getNow();
    }

    std::cout << defender << "(" << healthC->health << "/" << healthC->maxHealth << ") was dealt " << damageC->damage << " damage by " << attacker
              << std::endl;

    healthC->health = healthC->health - (damageC->damage - (armor != nullptr ? armor->armor : 0));
}

EntityID makePlayer(ECSManager* ECS, SpriteSystem* spriteSystem) {
    EntityID player = ECS->newEntity();

    const float startX = (float)(GetScreenWidth() * (1.0 / 3.0));
    const float startY = (float)(GetScreenHeight() / 1.5);

    Position::Component* position = ECS->addComp<Position::Component>(player, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(player, {Animation::AnimationID::Vortex, 2});
    ECS->addComp<Health::Component>(player, {50, 50});
    ECS->addComp<Player::Component>(player, {true});
    ECS->addComp<Armament::Component>(player, {Armament::Type::LaserBuckshot, 150, -1});
    ECS->addComp<Velocity::Component>(player, {10, 10});
    ECS->addComp<Hitbox::Component>(player, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(player, Team::Ally);
    ECS->addComp<ImmunityFrame::Component>(player, {1});
    ECS->addComp<Damage::Component>(player, {20});
    ECS->addComp<CollisionEffect::Component>(player, &dealDamage);

    return player;
}

void makeEndboss(ECSManager* ECS, SpriteSystem* spriteSystem) {
    EntityID endboss = ECS->newEntity();
    int players = 0;
    for (auto beg = ECS->begin<Player::Component>(); beg != ECS->end<Player::Component>(); ++beg) {
        players++;
    }

    Animation::Component* animation = ECS->addComp<Animation::Component>(endboss, {Animation::AnimationID::Cluster, 1, -90.0});

    float xOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameHeight * 3;

    float yOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameWidth / 2;

    const float startX = (float)(GetScreenWidth() - xOffset);

    const float startY = (float)(GetScreenHeight() - yOffset);

    Position::Component* position = ECS->addComp<Position::Component>(endboss, {startX, startY});

    ECS->addComp<Damage::Component>(endboss, {20});
    ECS->addComp<Health::Component>(endboss, {300 * players, 300 * players, false});
    ECS->addComp<Hitbox::Component>(endboss, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(endboss, Team::Enemy);
    ECS->addComp<CollisionEffect::Component>(endboss, &dealDamage);
}

void makeEnemy(ECSManager* ECS, SpriteSystem* spriteSystem) {
    EntityID enemy = ECS->newEntity();

    const float startX = (float)(GetScreenWidth());

    const float startY = (float)(rand() % GetScreenHeight());

    Position::Component* position = ECS->addComp<Position::Component>(enemy, {startX, startY});
    Animation::Component* animation = ECS->addComp<Animation::Component>(enemy, {Animation::AnimationID::Orb, 3});

    ECS->addComp<Velocity::Component>(enemy, {-10, 0});
    ECS->addComp<Health::Component>(enemy, {20, 20, true});
    ECS->addComp<Damage::Component>(enemy, {20});

    ECS->addComp<Hitbox::Component>(enemy, HitboxSystem::buildHitbox(animation, position));
    ECS->addComp<Team::Component>(enemy, Team::Enemy);
    ECS->addComp<Armament::Component>(enemy, {Armament::Type::Laser, 1000, 50});
    ECS->addComp<CollisionEffect::Component>(enemy, &dealDamage);
}

EntityID getPlayerID(ECSManager* ECS) {
    for (auto beg = ECS->begin<Player::Component>(); beg != ECS->end<Player::Component>(); ++beg) {
        return *beg;
    }
    return -1;
}

void bullet(ECSManager* ECS, SpriteSystem* spriteSystem, EntityID source, int velocityX, int velocityY, double rotation) {
    const EntityID bullet = ECS->newEntity();

    const Position::Component* sourcePos = ECS->getComponent<Position::Component>(source);
    const Velocity::Component* sourceVelocity = ECS->getComponent<Velocity::Component>(source);
    const Animation::Component* sourceAnimation = ECS->getComponent<Animation::Component>(source);
    const Point oldCenter{
        sourcePos->x + (float)Animation::Sheets[sourceAnimation->animationID].frameWidth * sourceAnimation->scale / 2.0,
        sourcePos->y + (float)Animation::Sheets[sourceAnimation->animationID].frameHeight * sourceAnimation->scale / 2.0,
    };

    Point center = HitboxSystem::rotate(oldCenter, {sourcePos->x, sourcePos->y}, HitboxSystem::toRadians(sourceAnimation->rotation));

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
    ECS->addComp<CollisionEffect::Component>(bullet, &dealDamage);
}

void makeLaser(ECSManager* ECS, SpriteSystem* spriteSystem, EntityID source) {
    bullet(ECS, spriteSystem, source, 40, 0, 0);
}

void makeLaserBuckshot(ECSManager* ECS, SpriteSystem* spriteSystem, EntityID source) {

    bullet(ECS, spriteSystem, source, 40, 0, 0);
    bullet(ECS, spriteSystem, source, 40, 5, 6);
    bullet(ECS, spriteSystem, source, 40, 10, 12);
    bullet(ECS, spriteSystem, source, 40, -5, -6);
    bullet(ECS, spriteSystem, source, 40, -10, -12);
}