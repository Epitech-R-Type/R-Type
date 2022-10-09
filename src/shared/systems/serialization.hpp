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
#include <iostream> // std::cout
#include <sstream>
#include <string> // std::string

std::string toString(Armor component) {
    std::stringstream ss;

    ss << "(" << component.armor << ")";
    return ss.str();
}

std::string toString(Health component) {
    std::stringstream ss;

    ss << "(" << component.health << ")";
    return ss.str();
}
std::string toString(Position component) {
    std::stringstream ss;

    ss << "(" << component.xpos << "," << component.ypos << ")";

    return ss.str();
}

template <class T>
std::string componentToString(EntityID entityId, Manager* manager) {
    T* component = manager->getComponent<T>(entityId);

    return std::to_string(getID<T>()) + toString(*component);
}

template <class... ComponentTypes>
std::string entityToString(EntityID entityID, Manager* manager) {
    Index entityIndex = getIndex(entityID);

    std::stringstream stream;

    stream << entityIndex << "(";

    bool first = true;
    for (const std::string component : {componentToString<ComponentTypes>(entityID, manager)...}) {
        if (!first)
            stream << ";";
        else
            first = false;

        stream << component;
    }

    stream << ")";

    return stream.str();
}

std::vector<std::string> argsSplit(std::string) {
    return std::vector<std::string>{"1"};
}

void stringToEntity(std::string entity, Manager* manager) {

    ComponentType componentTypeID = ComponentType::Armor_;
    EntityID entityID = 0;

    std::vector<std::string> components = {std::string("0"), std::string("1")};

    for (const std::string component : components) {

        std::vector<std::string> args = argsSplit(component);
        switch (componentTypeID) {
        case ComponentType::Armor_:
            Armor* component = manager->getComponent<Armor>(entityID);
            component->armor = stoi(args[0]);
        }
    }
}