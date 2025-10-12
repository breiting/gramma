#pragma once
#include <box2d/box2d.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/resource/Exit.hpp>
#include <gramma/model/resource/Home.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <memory>
#include <vector>

#include "gramma/core/SpatialGrid.hpp"

namespace gr {

/**
 * Global environment that contains all simulation data
 * Responsible for updating the simulation state and rendering.
 */

class Environment {
   public:
    Environment(const glm::vec2& gravity);
    ~Environment();

    /// Boundary
    void AddBoundary(const std::vector<glm::vec2>& vertices);
    const std::vector<glm::vec2>& GetBoundary() const;

    /// Obstacle
    void AddObstacle(const std::vector<glm::vec2>& contour);
    const std::vector<std::vector<glm::vec2>>& GetObstacles() const;

    /// Agents
    void AddAgent(std::unique_ptr<Agent> a);
    const Agent* GetAgent(size_t idx) const;
    const std::vector<std::unique_ptr<Agent>>& GetAgents() const;
    void RemoveAllAgents();

    /// Resources
    void AddResource(std::shared_ptr<IResource> r);
    const std::vector<std::shared_ptr<IResource>>& GetResources() const;

    /// Checks if a point is within the boundary
    bool ContainsPoint(const glm::vec2& p) const;
    glm::vec2 RandomPosition() const;

    /// Update and Render loop
    void Update(float dt);
    void Render(const glm::mat4& vp);

    /// Find the nearest resource given the type
    std::shared_ptr<IResource> FindNearest(ResourceType type, const glm::vec2& pos) const;

    /// Find agents (optimized)
    std::vector<const Agent*> QueryAgentsInRadius(const glm::vec2& center, float radius) const;

   private:
    void CreateChainShape(const std::vector<glm::vec2>& contour);

   private:
    b2WorldId m_World;
    std::vector<glm::vec2> m_Boundary;
    std::vector<std::vector<glm::vec2>> m_Obstacles;

    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::vector<std::shared_ptr<IResource>> m_Resources;

    std::unique_ptr<SpatialGrid> m_SpatialGrid;
};

}  // namespace gr
