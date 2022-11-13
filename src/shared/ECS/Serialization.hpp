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

    static ProtocolBuffer entityToBuffer(EntityID entityID, std::shared_ptr<ECSManager> manager);

    static EntityID bufferToEntity(const ProtocolBuffer buffer, std::shared_ptr<ECSManager> manager);

private:
    static ProtocolBuffer entityToBufferSwitch(EntityID entityID, std::shared_ptr<ECSManager> manager, ComponentType ID);

    template <typename T, typename M>
    static ProtocolBuffer componentToBuffer(EntityID entityId, std::shared_ptr<ECSManager> manager) {
        if (!manager->hasComponent<T>(entityId)) {
            ERRORLOG("Missing component: " << getID<T>());
            return {};
        }

        T component = *manager->getComponent<T>(entityId);

        return toBuffer<T, M>(component, ComponentType(getID<T>()));
    }
};
