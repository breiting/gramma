#include <cmath>
#include <gramma/core/SpatialGrid.hpp>

namespace gr {

SpatialGrid::SpatialGrid(float cellSize) : m_CellSize(cellSize) {
}

void SpatialGrid::Clear() {
    m_Grid.clear();
}

void SpatialGrid::Insert(size_t id, const glm::vec2& pos) {
    glm::ivec2 cell = GetCellIndex(pos);
    m_Grid[cell].push_back(id);
}

glm::ivec2 SpatialGrid::GetCellIndex(const glm::vec2& pos) const {
    return glm::ivec2(static_cast<int>(std::floor(pos.x / m_CellSize)),
                      static_cast<int>(std::floor(pos.y / m_CellSize)));
}

std::vector<size_t> SpatialGrid::QueryNeighborhood(const glm::vec2& pos, float radius) const {
    std::vector<size_t> result;

    int r = static_cast<int>(std::ceil(radius / m_CellSize));
    glm::ivec2 center = GetCellIndex(pos);

    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
            glm::ivec2 neighborCell = center + glm::ivec2(dx, dy);

            auto it = m_Grid.find(neighborCell);
            if (it != m_Grid.end()) {
                for (size_t id : it->second) {
                    result.push_back(id);
                }
            }
        }
    }

    return result;
}

}  // namespace gr
