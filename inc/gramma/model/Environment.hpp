#pragma once
#include <box2d/box2d.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/Types.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * Global environment that contains agents and food sources.
 * Responsible for updating the simulation state and rendering.
 */

class Environment {
   public:
    Environment(const glm::vec2& gravity);
    ~Environment();

    void AddBoundary(const std::vector<glm::vec2>& vertices);
    void AddObstacle(const std::vector<glm::vec2>& contour);

    const std::vector<glm::vec2>& GetBoundary() const;
    const std::vector<std::vector<glm::vec2>>& GetObstacles() const;

    bool ContainsPoint(const glm::vec2& p) const;
    glm::vec2 RandomPosition() const;

    void AddAgent(std::unique_ptr<Agent> a);
    void AddResource(std::shared_ptr<IResource> r);
    void AddHome(std::shared_ptr<Home> h);

    b2WorldId GetWorld() const {
        return m_World;
    }

    const std::vector<std::unique_ptr<Agent>>& Agents() const {
        return m_Agents;
    }
    const std::vector<std::shared_ptr<IResource>>& Resources() const {
        return m_Resources;
    }
    const std::vector<std::shared_ptr<Home>>& Homes() const {
        return m_Homes;
    }
    Home* GetNextFreeHome();

    // Update-Loop
    void Update(float dt);
    void Render(const glm::mat4& vp);

    void Stats() const;

    const Agent* GetAgent(size_t idx) const {
        return m_Agents[idx].get();
    }

    // Utility: nächste Ressource vom Typ
    std::shared_ptr<IResource> FindNearest(ResourceType type, const glm::vec2& pos) const;

   private:
    void CreateChainShape(const std::vector<glm::vec2>& contour);

   private:
    b2WorldId m_World;
    std::vector<glm::vec2> m_Boundary;
    std::vector<std::vector<glm::vec2>> m_Obstacles;

    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::vector<std::shared_ptr<IResource>> m_Resources;
    std::vector<std::shared_ptr<Home>> m_Homes;
};

}  // namespace gr
