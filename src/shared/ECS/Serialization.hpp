/*
** EPITECH PROJECT, 2022
** Manager.hpp
** File description:
** .
*/

#pragma once

#include "../ECS/Components.hpp"
#include "../ECS/ECS.hpp"
#include "../ECS/Manager.hpp"
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
        Index entityIndex = getIndex(entityID);
        std::stringstream stream;
        std::vector<std::string> cerealizedComponents = {Serialization::componentToString<ComponentTypes>(entityID, manager)...};

        stream << entityIndex << ";";

        if (cerealizedComponents.size() == 0) {
            const std::bitset<MAX_COMPONENTS> setComponents = manager->getSetComponents(entityID);

            for (int componentTypeID = 0; componentTypeID < MAX_COMPONENTS; componentTypeID++) {
                if (setComponents[componentTypeID] != 1)
                    continue;

                if (Serialization::hiddenComponents[componentTypeID]) {
                    std::cout << "Ooy ya cheeky wanker, ya'r not tryin to have a peak at hidden components are ya?" << std::endl;
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
                        std::cout << "[entityToString] Unhandled Component: " << componentTypeID << "." << std::endl;
                }
            }
        } else {
            for (const std::string component : cerealizedComponents) {
                stream << component;
            }
        }

        return stream.str();
    }

    static void stringToEntity(const std::string entity, std::shared_ptr<ECSManager> manager) {

        std::vector<std::string> components = Utilities::splitStr(entity, ";");

        EntityID entityID = atoi(components[0].c_str());

        for (auto beg = components.begin() + 1; beg != components.end(); beg++) {
            const std::string component = *beg;

            std::vector<std::string> args = Utilities::splitStr(component, ",");

            ComponentType componentTypeID = ComponentType(atoi(args[0].c_str()));

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
                    Position::applyUpdate(args, entityID, manager);
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
                    std::cout << "[stringToEntity] Unhandled Component: " << componentTypeID << "." << std::endl;
            }
        }
    }

    template <class T>
    static std::string componentToString(EntityID entityId, std::shared_ptr<ECSManager> manager) {
        if (!manager->hasComponent<T>(entityId)) {
            std::cout << "[Warning]: Missing component." << std::endl;
            return "";
        }

        T* component = manager->getComponent<T>(entityId);

        const Index componentTypeID = getID<T>();

        switch (componentTypeID) {
            case ComponentType::ARMOR:
                return std::to_string(getID<T>()) + "," + Armor::toString(*(Armor::Component*)component);
            case ComponentType::HEALTH:
                return std::to_string(getID<T>()) + "," + Health::toString(*(Health::Component*)component);
            case ComponentType::POSITION:
                return std::to_string(getID<T>()) + "," + Position::toString(*(Position::Component*)component);
            case ComponentType::ANIMATION:
                return std::to_string(getID<T>()) + "," + Animation::toString(*(Animation::Component*)component);
            case ComponentType::VELOCITY:
                return std::to_string(getID<T>()) + "," + Velocity::toString(*(Velocity::Component*)component);
            case ComponentType::PLAYER:
                return std::to_string(getID<T>()) + "," + Player::toString(*(Player::Component*)component);
            case ComponentType::DAMAGE:
                return std::to_string(getID<T>()) + "," + Damage::toString(*(Damage::Component*)component);
            case ComponentType::ARMAMENT:
                return std::to_string(getID<T>()) + "," + Armament::toString(*(Armament::Component*)component);
            case ComponentType::HITBOX:
                return std::to_string(getID<T>()) + "," + Hitbox::toString(*(Hitbox::Component*)component);
            case ComponentType::TEAM:
                return std::to_string(getID<T>()) + "," + Team::toString(*(Team::Component*)component);
            case ComponentType::IMMUNITY_FRAME:
                return std::to_string(getID<T>()) + "," + ImmunityFrame::toString(*(ImmunityFrame::Component*)component);
            case ComponentType::COLLISIONEFFECT:
                return std::to_string(getID<T>()) + "," + CollisionEffect::toString(*(CollisionEffect::Component*)component);
            default:
                std::cout << "[stringToEntity] Unhandled Component: " << componentTypeID << "." << std::endl;
                return "";
        }
    }
};
