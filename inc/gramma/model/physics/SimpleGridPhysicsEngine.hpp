#pragma once

#include <glm/vec2.hpp>
#include <gramma/model/physics/IPhysicsEngine.hpp>
#include <unordered_map>
#include <vector>

namespace gr {

class Agent;

class SimpleGridPhysicsEngine : public IPhysicsEngine {
   public:
    SimpleGridPhysicsEngine(float cellSize = 1.0f);
    void Init() override;
    void Step(float dt) override;

    void AddAgent(Agent& agent) override;
    void RemoveAgent(Agent& agent) override;

    void SyncAgentState(Agent& agent) override;
    void ApplyMovement(Agent& agent, const glm::vec2& movement) override;

    std::vector<Agent*> QueryAgentsInRadius(const glm::vec2& pos, float radius) const override;

   private:
    float m_CellSize;
    std::unordered_map<Agent*, glm::vec2> m_Positions;
    std::unordered_map<glm::ivec2, std::vector<Agent*>, std::hash<glm::ivec2>> m_Grid;

    glm::ivec2 getCell(const glm::vec2& pos) const;
    void rebuildGrid();
};

}  // namespace gr
