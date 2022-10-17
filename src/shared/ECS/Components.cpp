#include "Components.hpp"
#include "Manager.hpp"

std::string Armor::toString(Armor::Component component) {
    std::stringstream ss;

    ss << component.armor << ";";
    return ss.str();
}

void Armor::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Armor::Component>(entityID)) {
        Armor::Component* component = manager->getComponent<Armor::Component>(entityID);
        component->armor = stoi(args[1]);
    } else {
        manager->addComp<Armor::Component>(entityID, {stoi(args[1])});
    }
}

std::string Health::toString(Health::Component component) {
    std::stringstream ss;

    ss << component.health << ";";
    return ss.str();
}

void Health::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Health::Component>(entityID)) {
        Health::Component* component = manager->getComponent<Health::Component>(entityID);
        component->health = stoi(args[1]);
    } else {
        manager->addComp<Health::Component>(entityID, {stoi(args[1])});
    }
}

std::string Position::toString(Position::Component component) {
    std::stringstream ss;

    ss << component.x << "," << component.y << ";";
    return ss.str();
}

void Position::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Position::Component>(entityID)) {
        Position::Component* component = manager->getComponent<Position::Component>(entityID);
        component->x = strtof(args[1].c_str(), nullptr);
        component->y = strtof(args[2].c_str(), nullptr);
    } else {
        manager->addComp<Position::Component>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
    }
}

std::string Animation::toString(Animation::Component component) {
    std::stringstream ss;

    ss << component.animationID << ",";
    ss << component.layer << ",";
    ss << component.rotation << ",";
    ss << component.scale << ";";

    return ss.str();
}

void Animation::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Animation::Component>(entityID)) {
        Animation::Component* component = manager->getComponent<Animation::Component>(entityID);
        component->animationID = AnimationID(atoi(args[1].c_str()));
        component->layer = std::stoul(args[2].c_str(), nullptr);
    } else {
        manager->addComp<Animation::Component>(entityID, {AnimationID(atoi(args[2].c_str())), std::stoul(args[1].c_str())});
    }
}

std::string Velocity::toString(Velocity::Component component) {
    std::stringstream ss;

    ss << component.x << ",";
    ss << component.y << ",";
    ss << component.follow << ";";

    return ss.str();
}

void Velocity::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Velocity::Component>(entityID)) {
        Velocity::Component* component = manager->getComponent<Velocity::Component>(entityID);
        component->x = strtof(args[1].c_str(), nullptr);
        component->y = strtof(args[2].c_str(), nullptr);
    } else {
        manager->addComp<Velocity::Component>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
    }
}

std::string Player::toString(Player::Component component) {
    std::stringstream ss;

    ss << component.score << ";";

    return ss.str();
};

void Player::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Player::Component>(entityID)) {
        Player::Component* component = manager->getComponent<Player::Component>(entityID);
        component->score = atoi(args[1].c_str());
    } else {
        manager->addComp<Player::Component>(entityID, {atoi(args[1].c_str())});
    }
};

std::string Damage::toString(Damage::Component component) {
    std::stringstream ss;

    ss << component.damage << ";";

    return ss.str();
};

void Damage::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Damage::Component>(entityID)) {
        Damage::Component* component = manager->getComponent<Damage::Component>(entityID);
        component->damage = atoi(args[1].c_str());
    } else {
        manager->addComp<Damage::Component>(entityID, {atoi(args[1].c_str())});
    }
};

std::string Armament::toString(Armament::Component component) {
    std::stringstream ss;

    ss << component.type << ",";
    ss << component.interval << ",";
    ss << component.ammo << ";";

    return ss.str();
};

void Armament::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<Armament::Component>(entityID)) {
        Armament::Component* component = manager->getComponent<Armament::Component>(entityID);
        component->type = Armament::Type(atoi(args[1].c_str()));
        component->interval = strtof(args[1].c_str(), nullptr);
        component->ammo = atoi(args[1].c_str());
    } else {
        manager->addComp<Armament::Component>(entityID,
                                              {Armament::Type(atoi(args[1].c_str())), strtof(args[1].c_str(), nullptr), atoi(args[1].c_str())});
    }
};

std::string Hitbox::toString(Hitbox::Component component) {
    std::stringstream ss;

    ss << component.topLeft.x << ",";
    ss << component.topLeft.y << ",";
    ss << component.topRight.x << ",";
    ss << component.topRight.y << ",";
    ss << component.botLeft.x << ",";
    ss << component.botLeft.y << ",";
    ss << component.botRight.x << ",";
    ss << component.botRight.y << ";";

    return ss.str();
};

void Hitbox::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    Hitbox::Component* component;
    if (!manager->hasComponent<Hitbox::Component>(entityID))
        component = manager->addComp<Hitbox::Component>(entityID, {});
    else
        component = manager->getComponent<Hitbox::Component>(entityID);
    component->topLeft.x = std::strtof(args[0].c_str(), nullptr);
    component->topLeft.y = std::strtof(args[0].c_str(), nullptr);
    component->topRight.x = std::strtof(args[0].c_str(), nullptr);
    component->topRight.y = std::strtof(args[0].c_str(), nullptr);
    component->botLeft.x = std::strtof(args[0].c_str(), nullptr);
    component->botLeft.y = std::strtof(args[0].c_str(), nullptr);
    component->botRight.x = std::strtof(args[0].c_str(), nullptr);
    component->botRight.y = std::strtof(args[0].c_str(), nullptr);
};

std::string Team::toString(Team::Component component) {
    std::stringstream ss;

    ss << component << ";";

    return ss.str();
};

void Team::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    Team::Component* component;
    if (!manager->hasComponent<Team::Component>(entityID))
        component = manager->addComp<Team::Component>(entityID, {});
    else
        component = manager->getComponent<Team::Component>(entityID);
    *component = Team::Component(std::stoi(args[0].c_str(), nullptr));
};

std::string ImmunityFrame::toString(ImmunityFrame::Component component) {
    std::stringstream ss;

    ss << component.duration << ";";

    return ss.str();
};

void ImmunityFrame::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    ImmunityFrame::Component* component;
    if (!manager->hasComponent<ImmunityFrame::Component>(entityID))
        component = manager->addComp<ImmunityFrame::Component>(entityID, {});
    else
        component = manager->getComponent<ImmunityFrame::Component>(entityID);
    component->duration = std::strtod(args[0].c_str(), nullptr);
};

std::string CollisionEffect::toString(CollisionEffect::Component component) {
    std::stringstream ss;

    ss << ";";

    return ss.str();
};

void CollisionEffect::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    CollisionEffect::Component* component;
    if (!manager->hasComponent<CollisionEffect::Component>(entityID))
        component = manager->addComp<CollisionEffect::Component>(entityID, {});
    else
        component = manager->getComponent<CollisionEffect::Component>(entityID);
    *component = &CollisionEffect::dealDamage;
};

void CollisionEffect::dealDamage(EntityID attacker, EntityID defender, std::shared_ptr<ECSManager> ECS) {
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

    healthC->health = healthC->health - (damageC->damage - (armor != nullptr ? armor->armor : 0));
}
