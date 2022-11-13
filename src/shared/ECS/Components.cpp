#include "Components.hpp"

// ------------------

ComponentType getComponentType(ProtocolBuffer buffer) {
    return ComponentType(buffer[0]);
}

std::uint64_t getComponentSize(ProtocolBuffer buffer) {
    return *(std::uint64_t*)&buffer[1];
}

void CollisionEffect::dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS) {
    if (!ECS->hasComponent<Health::Component>(defender) || !ECS->getComponent<Damage::Component>(attacker))
        return;
    Health::Component* healthC = ECS->getComponent<Health::Component>(defender);
    ImmunityFrame::Component* immunityFrame = ECS->getComponent<ImmunityFrame::Component>(defender);
    Armor::Component* armor = ECS->getComponent<Armor::Component>(defender);
    Damage::Component* damageC = ECS->getComponent<Damage::Component>(attacker);

    if (healthC == nullptr || damageC == nullptr)
        return;

    if (immunityFrame != nullptr) {
        const auto now = getNow();
        std::chrono::duration<double> elapsed_seconds = now - immunityFrame->timer;

        if (elapsed_seconds.count() < immunityFrame->duration)
            return;
        immunityFrame->timer = getNow();
    }

    healthC->health = healthC->health - damageC->damage;
    ECS->pushModified(defender);
}
