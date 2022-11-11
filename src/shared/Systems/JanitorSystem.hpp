
#pragma once
#include <vector>

#include "../ECS/ECS.hpp"
#include "ISystem.hpp"

class JanitorSystem : public System {
public:
    /**
     * System that handles deletion of entities
     * */
    JanitorSystem(std::shared_ptr<ECSManager> ECS);

    /**
     * deletes all entities that match certain criteria such as:
     * - Out of Bounds
     * - Dead
     * */
    // Returns killed players
    void apply();

    // ─── Killed Player Handling ──────────────────────────────────────────────────────────────

    std::vector<int> getKilledPlayers() const;
    void resetKilledPlayers();

private:
    std::shared_ptr<ECSManager> _ECS;

    std::vector<int> _killedPlayers;
};