#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/agent/Agent.hpp>

namespace gr {

SpatialGrid::SpatialGrid(float cellSize) : m_CellSize(cellSize) {
}

void SpatialGrid::Clear() {
    m_Grid.clear();
    m_AgentToCell.clear();
}

glm::ivec2 SpatialGrid::GetCellIndex(const glm::vec2& pos) const {
    return glm::ivec2(static_cast<int>(std::floor(pos.x / m_CellSize)),
                      static_cast<int>(std::floor(pos.y / m_CellSize)));
}

void SpatialGrid::Insert(const std::string& id, const glm::vec2& pos, Agent* agent) {
    glm::ivec2 cell = GetCellIndex(pos);
    m_Grid[cell].push_back(agent);
    m_AgentToCell[id] = cell;
}

void SpatialGrid::Remove(const std::string& id, const glm::vec2& pos) {
    glm::ivec2 cell = GetCellIndex(pos);
    auto it = m_Grid.find(cell);
    if (it == m_Grid.end()) return;

    auto& vec = it->second;
    vec.erase(std::remove_if(vec.begin(), vec.end(), [id](Agent* a) { return a->GetId() == id; }), vec.end());

    if (vec.empty()) {
        m_Grid.erase(it);
    }

    m_AgentToCell.erase(id);
}

std::vector<const Agent*> SpatialGrid::QueryNeighborhood(const glm::vec2& pos, float radius) const {
    std::vector<const Agent*> result;

    int r = static_cast<int>(std::ceil(radius / m_CellSize));
    glm::ivec2 center = GetCellIndex(pos);
    float radiusSq = radius * radius;

    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
            glm::ivec2 neighbor = center + glm::ivec2(dx, dy);
            auto it = m_Grid.find(neighbor);
            if (it == m_Grid.end()) continue;

            for (Agent* a : it->second) {
                const glm::vec2& apos = a->GetPosition();
                if (glm::distance2(apos, pos) <= radiusSq) {
                    result.push_back(a);
                }
            }
        }
    }

    return result;
}

void SpatialGrid::SetBounds(const glm::vec2& min, const glm::vec2& max) {
    m_Min = min;
    m_Max = max;
}

bool SpatialGrid::IsInsideBounds(const glm::vec2& pos) const {
    return (pos.x >= m_Min.x && pos.x <= m_Max.x && pos.y >= m_Min.y && pos.y <= m_Max.y);
}

std::vector<Agent*> SpatialGrid::GetAllAgents() const {
    std::vector<Agent*> all;
    all.reserve(m_AgentToCell.size());

    for (const auto& [cell, list] : m_Grid) {
        all.insert(all.end(), list.begin(), list.end());
    }

    return all;
}

}  // namespace gr
