/*
** EPITECH PROJECT, 2022
** Serialization.cpp
** File description:
** .
*/

#include "Serialization.hpp"

std::bitset<MAX_COMPONENTS> Serialization::hiddenComponents;

ByteBuf Serialization::entityToBuffer(EntityID entityID, std::shared_ptr<ECSManager> manager) {
    const std::bitset<MAX_COMPONENTS> setComponents = manager->getSetComponents(entityID);

    ByteBuf buffer;

    buffer.resize(8 + 1);

    auto id = entityID;

    memcpy(&buffer[0], (char*)&id, 8);

    unsigned char compCount = 0;

    memcpy(&buffer[8], (char*)&compCount, sizeof(compCount));

    for (int componentTypeID = 0; componentTypeID < MAX_COMPONENTS; componentTypeID++) {
        if (setComponents[componentTypeID] != 1)
            continue;

        if (Serialization::hiddenComponents[componentTypeID]) {
            WARNING("Ooy ya cheeky wanker, ya'r not tryin to have a peak at hidden components are ya?");
            continue;
        }

        std::optional<ByteBuf> componentBuffer = entityToBufferSwitch(entityID, manager, ComponentType(componentTypeID));

        if (componentBuffer) {
            ByteBuf buf = componentBuffer.value();
            std::copy(buf.begin(), buf.end(), std::back_inserter(buffer));
            compCount++;
        } else
            break;
    }

    memcpy(&buffer[8], (char*)&compCount, sizeof(compCount));

    return buffer;
}

EntityID Serialization::bufferToEntity(const ByteBuf buffer, std::shared_ptr<ECSManager> manager) {
    const EntityID entityID = *(EntityID*)&buffer[0];

    if (!manager->isValidEntity(entityID))
        manager->newEntity(entityID);
    const std::uint8_t compCount = *(std::uint8_t*)&buffer[8];

    size_t nextCompPointer = 9;

    for (int i = 0; i < compCount; i++) {
        ByteBuf bufferComponent(buffer.begin() + nextCompPointer, buffer.end());

        switch (bufferComponent[0]) {
            case ComponentType::ARMOR:
                applyUpdate<Armor::Component, Armor::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::HEALTH:
                applyUpdate<Health::Component, Health::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::POSITION:
                applyUpdate<Position::Component, Position::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::ANIMATION:
                applyUpdate<Animation::Component, Animation::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::VELOCITY:
                applyUpdate<Velocity::Component, Velocity::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::PLAYER:
                applyUpdate<Player::Component, Player::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::DAMAGE:
                applyUpdate<Damage::Component, Damage::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::ARMAMENT:
                applyUpdate<Armament::Component, Armament::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::HITBOX:
                applyUpdate<Hitbox::Component, Hitbox::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::TEAM:
                applyUpdate<Team::Component, Team::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::IMMUNITY_FRAME:
                applyUpdate<ImmunityFrame::Component, ImmunityFrame::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::SOUND_CREATION:
                applyUpdate<SoundCreation::Component, SoundCreation::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::SOUND_DESTRUCTION:
                applyUpdate<SoundDestruction::Component, SoundDestruction::Mask>(bufferComponent, entityID, manager);
                break;
            case ComponentType::SOUND_DAMAGE:
                applyUpdate<SoundDamage::Component, SoundDamage::Mask>(bufferComponent, entityID, manager);
                break;
            default:
                ERRORLOG("Unhandled Component: " << bufferComponent[0] << ".");
        }
        nextCompPointer += 1 + 8 + getComponentSize(bufferComponent);
    }

    return entityID;
}

// ------------ THE SHADOW REALM ------------

std::optional<ByteBuf> Serialization::entityToBufferSwitch(EntityID entityID, std::shared_ptr<ECSManager> manager, ComponentType ID) {
    switch (ID) {
        case ComponentType::ARMOR:
            return Serialization::componentToBuffer<Armor::Component, Armor::Mask>(entityID, manager);
        case ComponentType::HEALTH:
            return Serialization::componentToBuffer<Health::Component, Health::Mask>(entityID, manager);
        case ComponentType::POSITION:
            return Serialization::componentToBuffer<Position::Component, Position::Mask>(entityID, manager);
        case ComponentType::ANIMATION:
            return Serialization::componentToBuffer<Animation::Component, Animation::Mask>(entityID, manager);
        case ComponentType::VELOCITY:
            return Serialization::componentToBuffer<Velocity::Component, Velocity::Mask>(entityID, manager);
        case ComponentType::PLAYER:
            return Serialization::componentToBuffer<Player::Component, Player::Mask>(entityID, manager);
        case ComponentType::DAMAGE:
            return Serialization::componentToBuffer<Damage::Component, Damage::Mask>(entityID, manager);
        case ComponentType::ARMAMENT:
            return Serialization::componentToBuffer<Armament::Component, Armament::Mask>(entityID, manager);
        case ComponentType::HITBOX:
            return Serialization::componentToBuffer<Hitbox::Component, Hitbox::Mask>(entityID, manager);
        case ComponentType::TEAM:
            return Serialization::componentToBuffer<Team::Component, Team::Mask>(entityID, manager);
        case ComponentType::IMMUNITY_FRAME:
            return Serialization::componentToBuffer<ImmunityFrame::Component, ImmunityFrame::Mask>(entityID, manager);
        case ComponentType::SOUND_CREATION:
            return Serialization::componentToBuffer<SoundCreation::Component, SoundCreation::Mask>(entityID, manager);
        case ComponentType::SOUND_DESTRUCTION:
            return Serialization::componentToBuffer<SoundDestruction::Component, SoundDestruction::Mask>(entityID, manager);
        case ComponentType::SOUND_DAMAGE:
            return Serialization::componentToBuffer<SoundDamage::Component, SoundDamage::Mask>(entityID, manager);
        default:
            ERRORLOG("Unhandled Component: " << ID << ".");
    }
}