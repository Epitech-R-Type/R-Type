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
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

template <class T>
std::string componentToString(EntityID entityId, Manager* manager) {
    if (!manager->hasComponent<T>(entityId)) {
        std::cout << "[Warning]: Missing component." << std::endl;
        return "";
    }

    T* component = manager->getComponent<T>(entityId);

    return std::to_string(getID<T>()) + "," + toString(*component);
}

template <class... ComponentTypes>
std::string entityToString(EntityID entityID, Manager* manager) {
    Index entityIndex = getIndex(entityID);

    std::stringstream stream;

    stream << entityIndex << ";";

    for (const std::string component : {componentToString<ComponentTypes>(entityID, manager)...}) {
        stream << component;
    }

    return stream.str();
}

std::vector<std::string> argsSplit(std::string component) {
    std::vector<std::string> args;
    const char* ptr = strtok((char*)(component.c_str()), ",");

    while (ptr != nullptr) {
        args.push_back(ptr);
        ptr = strtok(nullptr, ",");
    }

    return args;
}

std::vector<std::string> componentSplit(std::string entity) {
    std::vector<std::string> components;
    const char* ptr = strtok((char*)(entity.c_str()), ";");

    while (ptr != nullptr) {
        components.push_back(ptr);
        ptr = strtok(nullptr, ";");
    }

    return components;
}

void stringToEntity(const std::string entity, Manager* manager) {

    std::vector<std::string> components = componentSplit(entity);

    EntityID entityID = atoi(components[0].c_str());

    for (auto beg = components.begin() + 1; beg != components.end(); beg++) {
        const std::string component = *beg;

        std::vector<std::string> args = argsSplit(component);

        ComponentType componentTypeID = ComponentType(atoi(args[0].c_str()));

        switch (componentTypeID) {
            case ComponentType::ARMOR:
                Armor::applyUpdate(args, entityID, manager);
                break;
            case ComponentType::HEALTH:
                {
                    if (manager->hasComponent<Health>(entityID)) {
                        Health* component = manager->getComponent<Health>(entityID);
                        component->health = stoi(args[1]);
                    } else {
                        manager->addComp<Health>(entityID, {stoi(args[1])});
                    }
                    break;
                }
            case ComponentType::POSITION:
                {
                    if (manager->hasComponent<Position>(entityID)) {
                        Position* component = manager->getComponent<Position>(entityID);
                        component->xPos = strtof(args[1].c_str(), nullptr);
                        component->yPos = strtof(args[2].c_str(), nullptr);
                    } else {
                        manager->addComp<Position>(entityID, {strtof(args[1].c_str(), nullptr), strtof(args[2].c_str(), nullptr)});
                    }
                    break;
                }
        }
    }
}