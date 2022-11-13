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
#include "../Networking/ProtocolUtils.hpp"
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

    static ByteBuf entityToBuffer(EntityID entityID, std::shared_ptr<ECSManager> manager);

    static EntityID bufferToEntity(const ByteBuf buffer, std::shared_ptr<ECSManager> manager);

private:
    static ByteBuf entityToBufferSwitch(EntityID entityID, std::shared_ptr<ECSManager> manager, ComponentType ID);

    template <typename T, typename M>
    static ByteBuf componentToBuffer(EntityID entityId, std::shared_ptr<ECSManager> manager) {
        if (!manager->hasComponent<T>(entityId)) {
            ERRORLOG("Missing component: " << getID<T>());
            return {};
        }

        T component = *manager->getComponent<T>(entityId);

        return toBuffer<T, M>(component, ComponentType(getID<T>()));
    }
};
