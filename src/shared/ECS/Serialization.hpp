/*
** EPITECH PROJECT, 2022
** Manager.hpp
** File description:
** .
*/

#pragma once

#include "../ECS/Components.hpp"
#include "../ECS/ECS.hpp"
#include "../ECS/ECSManager.hpp"
#include "../Utilities/Utilities.hpp"
#include <cstring>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

class Serialization {
public:
    static std::bitset<MAX_COMPONENTS> hiddenComponents;

    template <class... ComponentTypes>
    static std::string entityToString(EntityID entityID, std::shared_ptr<ECSManager> manager) {
        std::stringstream stream;
        std::vector<std::string> cerealizedComponents = {Serialization::componentToString<ComponentTypes>(entityID, manager)...};

        stream << entityID << ";";

        if (cerealizedComponents.size() == 0) {
            const std::bitset<MAX_COMPONENTS> setComponents = manager->getSetComponents(entityID);

            for (int componentTypeID = 0; componentTypeID < MAX_COMPONENTS; componentTypeID++) {
                if (setComponents[componentTypeID] != 1)
                    continue;

                if (Serialization::hiddenComponents[componentTypeID]) {
                    WARNING("Ooy ya cheeky wanker, ya'r not tryin to have a peak at hidden components are ya?");
                    continue;
                }

                switch (componentTypeID) {
                    case ComponentType::ARMOR:
                        stream << Serialization::componentToString<Armor::Component>(entityID, manager);
                        break;
                    case ComponentType::HEALTH:
                        stream << Serialization::componentToString<Health::Component>(entityID, manager);
                        break;
                    case ComponentType::POSITION:
                        stream << Serialization::componentToString<Position::Component>(entityID, manager);
                        break;
                    case ComponentType::ANIMATION:
                        stream << Serialization::componentToString<Animation::Component>(entityID, manager);
                        break;
                    case ComponentType::VELOCITY:
                        stream << Serialization::componentToString<Velocity::Component>(entityID, manager);
                        break;
                    case ComponentType::PLAYER:
                        stream << Serialization::componentToString<Player::Component>(entityID, manager);
                        break;
                    case ComponentType::DAMAGE:
                        stream << Serialization::componentToString<Damage::Component>(entityID, manager);
                        break;
                    case ComponentType::ARMAMENT:
                        stream << Serialization::componentToString<Armament::Component>(entityID, manager);
                        break;
                    case ComponentType::HITBOX:
                        stream << Serialization::componentToString<Hitbox::Component>(entityID, manager);
                        break;
                    case ComponentType::TEAM:
                        stream << Serialization::componentToString<Team::Component>(entityID, manager);
                        break;
                    case ComponentType::IMMUNITY_FRAME:
                        stream << Serialization::componentToString<ImmunityFrame::Component>(entityID, manager);
                        break;
                    case ComponentType::COLLISIONEFFECT:
                        stream << Serialization::componentToString<CollisionEffect::Component>(entityID, manager);
                        break;
                    default:
                        ERROR("Unhandled Component: " << componentTypeID << ".");
                }
            }
        } else {
            for (const std::string component : cerealizedComponents) {
                stream << component;
            }
        }

        return stream.str();
    }

    static EntityID stringToEntity(const std::string entity, std::shared_ptr<ECSManager> manager) {
        std::vector<std::string> components = Utilities::splitStr(entity, ";");

        EntityID entityID = std::stoll(components[0]);

        if (!manager->isValidEntity(entityID))
            manager->newEntity(entityID);

        for (auto beg = components.begin() + 1; beg != components.end() && (*beg)[(*beg).size() - 1] != '\n'; beg++) {
            const std::string component = *beg;
            std::vector<std::string> args = Utilities::splitStr(component, ",");

            ComponentType componentTypeID = ComponentType(std::stol(args[0]));

            switch (componentTypeID) {
                case ComponentType::ARMOR:
                    Armor::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::HEALTH:
                    Health::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::POSITION:
                    Position::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::ANIMATION:
                    Animation::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::VELOCITY:
                    Velocity::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::PLAYER:
                    Player::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::DAMAGE:
                    Damage::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::ARMAMENT:
                    Armament::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::HITBOX:
                    Hitbox::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::TEAM:
                    Team::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::IMMUNITY_FRAME:
                    ImmunityFrame::applyUpdate(args, entityID, manager);
                    break;
                case ComponentType::COLLISIONEFFECT:
                    CollisionEffect::applyUpdate(args, entityID, manager);
                    break;
                default:
                    ERROR("Unhandled Component: " << componentTypeID << ".");
            }
        }

        return entityID;
    }

    template <class T>
    static std::string componentToString(EntityID entityId, std::shared_ptr<ECSManager> manager) {
        if (!manager->hasComponent<T>(entityId)) {
            ERROR("Missing component: " << getID<T>());
            return "";
        }

        T* component = manager->getComponent<T>(entityId);

        return std::to_string(getID<T>()) + "," + toString(*component);
    }
};
