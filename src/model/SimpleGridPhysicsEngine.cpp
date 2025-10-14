#include <glm/gtx/norm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/physics/SimpleGridPhysicsEngine.hpp>

namespace gr {

SimpleGridPhysicsEngine::SimpleGridPhysicsEngine(float cellSize) : m_CellSize(cellSize) {
    m_SpatialGrid = std::make_unique<SpatialGrid<Agent*>>(m_CellSize);
}

void SimpleGridPhysicsEngine::Init() {
    // Initialize with default world bounds
    m_SpatialGrid->SetBounds(m_MinBounds, m_MaxBounds);
}

void SimpleGridPhysicsEngine::Step(float /*dt*/) {
    // ⚡ Ultra-fast incremental grid update
    // No full rebuild – SpatialGrid already reflects current positions
}

void SimpleGridPhysicsEngine::AddBoundary(const std::vector<glm::vec2>& contour) {
    if (contour.empty()) return;

    glm::vec2 min = contour[0];
    glm::vec2 max = contour[0];

    for (const auto& p : contour) {
        min = glm::min(min, p);
        max = glm::max(max, p);
    }

    m_MinBounds = min;
    m_MaxBounds = max;
    m_SpatialGrid->SetBounds(min, max);
}

void SimpleGridPhysicsEngine::AddObstacle(const std::vector<glm::vec2>& /*contour*/) {
    // Optional: Could be added as forbidden cells later
}

void SimpleGridPhysicsEngine::AddResource(std::shared_ptr<IResource> /*r*/) {
    // Physics does not manage resources
}

void SimpleGridPhysicsEngine::RemoveResource(IResource* /*r*/) {
    // Physics does not manage resources
}

void SimpleGridPhysicsEngine::AddAgent(Agent& agent) {
    m_SpatialGrid->Insert(agent.GetId(), agent.GetPosition(), &agent);
}

void SimpleGridPhysicsEngine::RemoveAgent(Agent& agent) {
    m_SpatialGrid->Remove(agent.GetId(), agent.GetPosition());
}

void SimpleGridPhysicsEngine::SyncAgentState(Agent& /*agent*/) {
    // Nothing to sync; agent positions are already authoritative
}

void SimpleGridPhysicsEngine::ApplyMovement(Agent& agent, const glm::vec2& movement) {
    glm::vec2 pos = agent.GetPosition();
    glm::vec2 newPos = pos + movement;

    // --- Boundary clamp & bounce ---
    if (!m_SpatialGrid->IsInsideBounds(newPos)) {
        glm::vec2 min = m_SpatialGrid->GetMinBounds();
        glm::vec2 max = m_SpatialGrid->GetMaxBounds();

        if (newPos.x < min.x || newPos.x > max.x) {
            newPos.x = pos.x - movement.x;
        }
        if (newPos.y < min.y || newPos.y > max.y) {
            newPos.y = pos.y - movement.y;
        }

        newPos = glm::clamp(newPos, min, max);
    }

    // --- Update grid position ---
    m_SpatialGrid->Remove(agent.GetId(), pos);
    agent.SetPosition(newPos);
    agent.SetHeading(newPos - pos);
    m_SpatialGrid->Insert(agent.GetId(), newPos, &agent);
}

std::vector<const Agent*> SimpleGridPhysicsEngine::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    auto nearby = m_SpatialGrid->QueryNeighborhood(pos, radius);

    std::vector<const Agent*> result;
    result.reserve(nearby.size());

    for (Agent* ptr : nearby) {
        result.push_back(static_cast<const Agent*>(ptr));
    }

    return result;
}

}  // namespace gr
