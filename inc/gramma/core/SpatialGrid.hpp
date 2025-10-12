#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

namespace gr {

struct CellHasher {
    std::size_t operator()(const glm::ivec2& cell) const {
        // return std::hash<int>()(cell.x) ^ (std::hash<int>()(cell.y) << 1);
        return std::hash<int>()(cell.x * 73856093 ^ cell.y * 19349663);
    }
};

class SpatialGrid {
   public:
    SpatialGrid(float cellSize);

    void Clear();
    void Insert(size_t id, const glm::vec2& pos);
    std::vector<size_t> QueryNeighborhood(const glm::vec2& pos, float radius) const;

   private:
    float m_CellSize;
    std::unordered_map<glm::ivec2, std::vector<size_t>, CellHasher> m_Grid;

    glm::ivec2 GetCellIndex(const glm::vec2& pos) const;
};

}  // namespace gr
