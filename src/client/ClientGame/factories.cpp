#include "factories.hpp"
#include "../../shared/ECS/Manager.hpp"
#include "../systems/SpriteSystem.hpp"

EntityID makePlayer(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID player = ECS->newEntity();

    ECS->addComp<Position::Component>(player, {(float)(GetScreenWidth() * (2.0 / 3.0)), (float)(GetScreenHeight() - 50)});
    ECS->addComp<Animation::Component>(player, {Animation::AnimationID::Vortex, 2});
    ECS->addComp<Health::Component>(player, {25, 30});
    ECS->addComp<Player::Component>(player, {true});
    ECS->addComp<Armament::Component>(player, {Armament::ArmamentType::Bullet, 150, 50});
    ECS->addComp<Velocity::Component>(player, {10, 10});

    return player;
}

void makeEndboss(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID endboss = ECS->newEntity();
    int players = 3;

    ECS->addComp<Animation::Component>(endboss, {Animation::AnimationID::Cluster, 1, 1});
    ECS->addComp<Health::Component>(endboss, {300 * players, 300 * players, false});

    float xOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameHeight * 3 / 2;

    float yOffset = Animation::Sheets[Animation::AnimationID::Cluster].frameWidth * 3 / 2;

    ECS->addComp<Position::Component>(endboss, {(float)(GetScreenWidth() - xOffset), (float)(GetScreenHeight() - yOffset)});
}

void makeEnemy(Manager* ECS, SpriteSystem* spriteSystem) {
    EntityID enemy = ECS->newEntity();

    ECS->addComp<Position::Component>(enemy, {(float)(GetScreenWidth()), (float)(rand() % GetScreenHeight())});
    ECS->addComp<Animation::Component>(enemy, {Animation::AnimationID::Orb, 3});
    ECS->addComp<Velocity::Component>(enemy, {-3, 0});
    ECS->addComp<Health::Component>(enemy, {10, 20, true});
    ECS->addComp<Damage::Component>(enemy, {20});
}

EntityID getPlayerID(Manager* ECS) {
    for (auto beg = ECS->begin<Player::Component>(); beg != ECS->end<Player::Component>(); ++beg) {
        return *beg;
    }
    return -1;
}

void makeBullet(Manager* ECS, SpriteSystem* spriteSystem) {
    const EntityID bullet = ECS->newEntity();

    const EntityID player = getPlayerID(ECS);

    ECS->addComp<Position::Component>(bullet, *ECS->getComponent<Position::Component>(player));
    ECS->addComp<Velocity::Component>(bullet, {40, 0});
    ECS->addComp<Damage::Component>(bullet, {10});
    ECS->addComp<Animation::Component>(bullet, {Animation::AnimationID::Laser});
}