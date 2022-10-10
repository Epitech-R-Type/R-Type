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

std::vector<std::string> argsSplit(const std::string component) {
    std::regex argsRegex("\\(.*\\)", std::regex_constants::ECMAScript);
    std::smatch match;

    std::regex_search(component.begin(), component.end(), match, argsRegex);
    std::vector<std::string> args;
    std::regex argRegex("\\d+(\\.\\d+)?", std::regex_constants::ECMAScript);

    std::sregex_iterator words_begin(match.str().begin(), match.str().end(), argRegex);
    std::sregex_iterator words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        args.push_back((*i).str());
    }

    return args;
}

std::vector<std::string> getComponents(const std::string entity) {
    std::vector<std::string> components;
    std::regex componentRegex("\\d+\\((\\d+,?)+\\)", std::regex_constants::ECMAScript);

    std::sregex_iterator words_begin(entity.begin(), entity.end(), componentRegex);
    std::sregex_iterator words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        components.push_back((*i).str());
    }

    return components;
}

void stringToEntity(const std::string entity, Manager* manager) {

    std::regex idRegex("\\d+", std::regex_constants::ECMAScript);
    std::smatch match;

    std::regex_search(entity.begin(), entity.end(), match, idRegex);

    EntityID entityID = atoi(match.str().c_str());

    std::vector<std::string> components = getComponents(entity);

    for (const std::string component : components) {
        std::regex entityIDRegex("\\d+", std::regex_constants::ECMAScript);
        std::smatch entityIDMatch;

        std::regex_search(component.begin(), component.end(), entityIDMatch, entityIDRegex);

        ComponentType componentTypeID = ComponentType(atoi(entityIDMatch.str().c_str()));

        std::vector<std::string> args = argsSplit(component);

        switch (componentTypeID) {
        case ComponentType::ARMOR: {
            Armor* component = manager->getComponent<Armor>(entityID);
            component->armor = stoi(args[0]);
            break;
        }
        case ComponentType::HEALTH: {
            Health* component = manager->getComponent<Health>(entityID);
            component->health = stoi(args[0]);
            break;
        }
        case ComponentType::POSITION: {
            Position* component = manager->getComponent<Position>(entityID);
            component->xpos = strtof(args[0].c_str(), nullptr);
            component->ypos = strtof(args[1].c_str(), nullptr);
            break;
        }
        }
    }
}