#include "Components.hpp"
#include "Manager.hpp"

std::string Armor::toString(Armor::Component component) {
    std::stringstream ss;

    ss << component.armor << ";";
    return ss.str();
}

void Armor::applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
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

void Health::applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
    if (manager->hasComponent<Health::Component>(entityID)) {
        Health::Component* component = manager->getComponent<Health::Component>(entityID);
        component->health = stoi(args[1]);
    } else {
        manager->addComp<Health::Component>(entityID, {stoi(args[1])});
    }
}

std::string Position::toString(Position::Component component) {
    std::stringstream ss;

    ss << component.xPos << "," << component.yPos << ";";
    return ss.str();
}

void Position::applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
    if (manager->hasComponent<Position::Component>(entityID)) {
        Position::Component* component = manager->getComponent<Position::Component>(entityID);
        component->xPos = strtof(args[1].c_str(), nullptr);
        component->yPos = strtof(args[2].c_str(), nullptr);
    } else {
        manager->addComp<Position::Component>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
    }
}

std::string Animation::toString(Animation::Component component) {
    std::stringstream ss;

    ss << component.animationID << "," << component.layer << ";";
    return ss.str();
}

void Animation::applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
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

    ss << component.xVelocity << "," << component.yVelocity << ";";
    return ss.str();
}

void Velocity::applyUpdate(std::vector<std::string> args, EntityID entityID, Manager* manager) {
    if (manager->hasComponent<Velocity::Component>(entityID)) {
        Velocity::Component* component = manager->getComponent<Velocity::Component>(entityID);
        component->xVelocity = strtof(args[1].c_str(), nullptr);
        component->yVelocity = strtof(args[2].c_str(), nullptr);
    } else {
        manager->addComp<Velocity::Component>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
    }
}

std::string Player::toString(Player::Component component) {
    std::stringstream ss;

    ss << component.isPlayer << ";";
    return ss.str();
}
