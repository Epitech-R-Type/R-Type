#include "Components.hpp"
#include "ECSManager.hpp"

std::string toString(Armor::Component component) {
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

std::string toString(Health::Component component) {
    std::stringstream ss;

    ss << component.health << ",";
    ss << component.maxHealth << ",";
    ss << component.visible << ";";
    return ss.str();
}

void Health::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    Health::Component* component;
    if (manager->hasComponent<Health::Component>(entityID))
        component = manager->getComponent<Health::Component>(entityID);
    else
        component = manager->addComp<Health::Component>(entityID, {stoi(args[1])});
    component->health = stoi(args[1]);
    component->maxHealth = stoi(args[2]);
    component->visible = stoi(args[3]);
}

std::string toString(Position::Component component) {
    std::stringstream ss;

    ss << component.x << ",";
    ss << component.y << ";";
    return ss.str();
}

void Position::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    Position::Component* component;
    if (manager->hasComponent<Position::Component>(entityID))
        component = manager->getComponent<Position::Component>(entityID);
    else
        component = manager->addComp<Position::Component>(entityID, {});
    component->x = strtof(args[1].c_str(), nullptr);
    component->y = strtof(args[2].c_str(), nullptr);
}

std::string toString(Animation::Component component) {
    std::stringstream ss;

    ss << component.animationID << ",";
    ss << component.layer << ",";
    ss << component.rotation << ",";
    ss << component.scale << ";";

    return ss.str();
}

void Animation::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    Animation::Component* component;
    if (manager->hasComponent<Animation::Component>(entityID)) {
        component = manager->getComponent<Animation::Component>(entityID);
    } else {
        component = manager->addComp<Animation::Component>(entityID, {});
    }
    component->animationID = AnimationID(atoi(args[1].c_str()));
    component->layer = std::stoul(args[2].c_str(), nullptr);
    component->rotation = std::stod(args[3].c_str(), nullptr);
    component->scale = std::stod(args[4].c_str(), nullptr);
}

std::string toString(Velocity::Component component) {
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

std::string toString(Player::Component component) {
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

std::string toString(Damage::Component component) {
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

std::string toString(Armament::Component component) {
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

std::string toString(Hitbox::Component component) {
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

std::string toString(Team::Component component) {
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

std::string toString(ImmunityFrame::Component component) {
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

std::string toString(CollisionEffect::Component component) {
    std::stringstream ss;

    ss << ";";

    return ss.str();
};

void CollisionEffect::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    CollisionEffect::Component* component;
    if (!manager->hasComponent<CollisionEffect::Component>(entityID))
        component = manager->addComp<CollisionEffect::Component>(entityID, &CollisionEffect::dealDamage);
    else
        component = manager->getComponent<CollisionEffect::Component>(entityID);
    // *component = &CollisionEffect::dealDamage;
};

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

std::string toString(SoundCreation::Component component) {
    std::stringstream ss;

    ss << component.ID << ";";

    return ss.str();
}

void SoundCreation::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<SoundCreation::Component>(entityID)) {
        SoundCreation::Component* component = manager->getComponent<SoundCreation::Component>(entityID);
        component->ID = SFXID::INVALID;
    } else {
        manager->addComp<SoundCreation::Component>(entityID, {(SFXID)stoi(args[1])});
    }
}

std::string toString(SoundDestruction::Component component) {
    std::stringstream ss;

    ss << component.ID << ";";

    return ss.str();
}

void SoundDestruction::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<SoundDestruction::Component>(entityID)) {
        SoundDestruction::Component* component = manager->getComponent<SoundDestruction::Component>(entityID);
        component->ID = (SFXID)stoi(args[1]);
    } else {
        std::cout << "created" << std::endl;
        manager->addComp<SoundDestruction::Component>(entityID, {(SFXID)stoi(args[1])});
    }
}

std::string toString(SoundDamage::Component component) {
    std::stringstream ss;

    ss << component.ID << ";";

    return ss.str();
}

void SoundDamage::applyUpdate(std::vector<std::string> args, EntityID entityID, std::shared_ptr<ECSManager> manager) {
    if (manager->hasComponent<SoundDamage::Component>(entityID)) {
        SoundDamage::Component* component = manager->getComponent<SoundDamage::Component>(entityID);
        component->ID = (SFXID)stoi(args[1]);
    } else {
        manager->addComp<SoundDamage::Component>(entityID, {(SFXID)stoi(args[1])});
    }
}