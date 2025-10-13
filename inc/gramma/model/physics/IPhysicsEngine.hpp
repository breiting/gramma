// IPhysicsEngine.hpp
#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace gr {

class Agent;
class IResource;

class IPhysicsEngine {
   public:
    virtual ~IPhysicsEngine() = default;

    virtual void Init() = 0;
    virtual void Step(float dt) = 0;

    virtual void AddBoundary(const std::vector<glm::vec2>& contour) = 0;
    virtual void AddObstacle(const std::vector<glm::vec2>& contour) = 0;
    virtual void AddResource(std::shared_ptr<IResource> r) = 0;
    virtual void RemoveResource(IResource* r) = 0;

    virtual void AddAgent(Agent& agent) = 0;
    virtual void RemoveAgent(Agent& agent) = 0;

    virtual void SyncAgentState(Agent& agent) = 0;
    virtual void ApplyMovement(Agent& agent, const glm::vec2& movement) = 0;

    virtual std::vector<const Agent*> QueryAgentsInRadius(const glm::vec2& pos, float radius) const = 0;
};

}  // namespace gr
