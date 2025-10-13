#pragma once

#include <box2d/box2d.h>

#include <glm/vec2.hpp>
#include <gramma/model/physics/IPhysicsEngine.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <unordered_map>

namespace gr {

class Agent;

class Box2DPhysicsEngine : public IPhysicsEngine {
   public:
    Box2DPhysicsEngine();
    ~Box2DPhysicsEngine() override;

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
    void CreateChainShape(const std::vector<glm::vec2>& contour);

   private:
    b2WorldId m_World;
    std::unordered_map<Agent*, b2BodyId> m_Bodies;

    b2BodyId CreateBodyForAgent(Agent& agent);
};

}  // namespace gr
