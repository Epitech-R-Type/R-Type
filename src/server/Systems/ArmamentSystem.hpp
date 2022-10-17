#pragma once

#include "../../shared/ECS/Components.hpp"
#include "../../shared/ECS/ECS.hpp"
#include "../../shared/Systems/ISystem.hpp"
#include <memory>

class ArmamentSystem : public System {
public:
    ArmamentSystem(std::shared_ptr<ECSManager> ECS);

    void apply();

    static void makeWeapon(EntityID entityID, std::shared_ptr<ECSManager> ECS);

private:
    std::shared_ptr<ECSManager> _ECS;
};