#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/physics/SimpleGridPhysicsEngine.hpp>

namespace gr {

SimpleGridPhysicsEngine::SimpleGridPhysicsEngine(float cellSize) : m_CellSize(cellSize) {
}

void SimpleGridPhysicsEngine::Init() {
}

void SimpleGridPhysicsEngine::Step(float /*dt*/) {
    rebuildGrid();
}

void SimpleGridPhysicsEngine::AddAgent(Agent& agent) {
    m_Positions[&agent] = agent.GetPosition();
}

void SimpleGridPhysicsEngine::RemoveAgent(Agent& agent) {
    m_Positions.erase(&agent);
}

void SimpleGridPhysicsEngine::SyncAgentState(Agent& agent) {
    auto it = m_Positions.find(&agent);
    if (it != m_Positions.end()) {
        agent.SetPosition(it->second);
    }
}

void SimpleGridPhysicsEngine::ApplyMovement(Agent& agent, const glm::vec2& movement) {
    m_Positions[&agent] += movement;
}

std::vector<Agent*> SimpleGridPhysicsEngine::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    std::vector<Agent*> result;
    int r = static_cast<int>(std::ceil(radius / m_CellSize));

    glm::ivec2 center = getCell(pos);

    for (int dx = -r; dx <= r; ++dx) {
        for (int dy = -r; dy <= r; ++dy) {
            glm::ivec2 cell = center + glm::ivec2(dx, dy);
            auto it = m_Grid.find(cell);
            if (it != m_Grid.end()) {
                for (Agent* agent : it->second) {
                    if (glm::distance(m_Positions.at(agent), pos) <= radius) {
                        result.push_back(agent);
                    }
                }
            }
        }
    }

    return result;
}

glm::ivec2 SimpleGridPhysicsEngine::getCell(const glm::vec2& pos) const {
    return glm::ivec2(static_cast<int>(std::floor(pos.x / m_CellSize)),
                      static_cast<int>(std::floor(pos.y / m_CellSize)));
}

void SimpleGridPhysicsEngine::rebuildGrid() {
    m_Grid.clear();
    for (const auto& [agent, pos] : m_Positions) {
        m_Grid[getCell(pos)].push_back(agent);
    }
}

}  // namespace gr
