#pragma once

#include <algorithm>
#include <cstddef>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>

namespace gr {

class Agent;

struct CellHasher {
    std::size_t operator()(const glm::ivec2& v) const noexcept {
        // Hash function for grid indexing
        return std::hash<int>()(v.x * 73856093) ^ std::hash<int>()(v.y * 19349663);
    }
};

/**
 * SpatialGrid
 * Uniform grid for fast spatial neighborhood queries (O(1) average access)
 * Scales to millions of agents efficiently
 */
class SpatialGrid {
   public:
    explicit SpatialGrid(float cellSize);

    void Clear();
    void Insert(const std::string& id, const glm::vec2& pos, Agent* agent);
    void Remove(const std::string& id, const glm::vec2& pos);

    std::vector<const Agent*> QueryNeighborhood(const glm::vec2& pos, float radius) const;

    void SetBounds(const glm::vec2& min, const glm::vec2& max);
    bool IsInsideBounds(const glm::vec2& pos) const;

    const glm::vec2& GetMinBounds() const {
        return m_Min;
    }
    const glm::vec2& GetMaxBounds() const {
        return m_Max;
    }

    std::vector<Agent*> GetAllAgents() const;

   private:
    glm::ivec2 GetCellIndex(const glm::vec2& pos) const;

   private:
    float m_CellSize;
    glm::vec2 m_Min{-1000.f};
    glm::vec2 m_Max{1000.f};

    // Grid mapping cell -> list of agent pointers
    std::unordered_map<glm::ivec2, std::vector<Agent*>, CellHasher> m_Grid;

    // Optional: store IDs if needed
    std::unordered_map<std::string, glm::ivec2> m_AgentToCell;
};

}  // namespace gr
