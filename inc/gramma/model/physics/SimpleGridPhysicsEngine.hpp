#pragma once

#include <glm/vec2.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/physics/IPhysicsEngine.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <memory>
#include <vector>

namespace gr {

class Agent;

/**
 * Extremely lightweight physics engine for agent-based simulations.
 * Uses SpatialGrid for efficient neighborhood queries and boundary control.
 * Designed to handle millions of agents in real time.
 */
class SimpleGridPhysicsEngine : public IPhysicsEngine {
   public:
    explicit SimpleGridPhysicsEngine(float cellSize = 1.0f);

    void Init() override;
    void Step(float dt) override;

    void AddBoundary(const std::vector<glm::vec2>& contour) override;
    void AddObstacle(const std::vector<glm::vec2>& contour) override;
    void AddResource(std::shared_ptr<IResource> r) override;
    void RemoveResource(IResource* r) override;

    void AddAgent(Agent& agent) override;
    void RemoveAgent(Agent& agent) override;

    void SyncAgentState(Agent& agent) override;
    void ApplyMovement(Agent& agent, const glm::vec2& movement) override;

    std::vector<const Agent*> QueryAgentsInRadius(const glm::vec2& pos, float radius) const override;

   private:
    float m_CellSize;
    std::unique_ptr<SpatialGrid<Agent*>> m_SpatialGrid;

    glm::vec2 m_MinBounds{-1000.f, -1000.f};
    glm::vec2 m_MaxBounds{1000.f, 1000.f};
};

}  // namespace gr
