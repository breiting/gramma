#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <gramma/core/SpatialGrid.hpp>

namespace gr {

template <typename T>
SpatialGrid<T>::SpatialGrid(float cellSize) : m_CellSize(cellSize) {
}

template <typename T>
void SpatialGrid<T>::Clear() {
    m_Grid.clear();
    m_ItemToCell.clear();
}

template <typename T>
glm::ivec2 SpatialGrid<T>::GetCellIndex(const glm::vec2& pos) const {
    return glm::ivec2(static_cast<int>(std::floor(pos.x / m_CellSize)),
                      static_cast<int>(std::floor(pos.y / m_CellSize)));
}

template <typename T>
void SpatialGrid<T>::Insert(const std::string& id, const glm::vec2& pos, T item) {
    glm::ivec2 cell = GetCellIndex(pos);
    m_Grid[cell].push_back(item);
    m_ItemToCell[id] = cell;
}

template <typename T>
void SpatialGrid<T>::Remove(const std::string& id, const glm::vec2& pos) {
    glm::ivec2 cell = GetCellIndex(pos);
    auto it = m_Grid.find(cell);
    if (it == m_Grid.end()) return;

    auto& vec = it->second;
    vec.erase(std::remove_if(vec.begin(), vec.end(), [id](T a) { return a->GetId() == id; }), vec.end());

    if (vec.empty()) {
        m_Grid.erase(it);
    }

    m_ItemToCell.erase(id);
}

template <typename T>
std::vector<const T> SpatialGrid<T>::QueryNeighborhood(const glm::vec2& pos, float radius) const {
    std::vector<const T> result;

    int r = static_cast<int>(std::ceil(radius / m_CellSize));
    glm::ivec2 center = GetCellIndex(pos);
    float radiusSq = radius * radius;

    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx <= r; ++dx) {
            glm::ivec2 neighbor = center + glm::ivec2(dx, dy);
            auto it = m_Grid.find(neighbor);
            if (it == m_Grid.end()) continue;

            for (T* item : it->second) {
                const glm::vec2& apos = item->GetPosition();
                if (glm::distance2(apos, pos) <= radiusSq) {
                    result.push_back(item);
                }
            }
        }
    }

    return result;
}

template <typename T>
void SpatialGrid<T>::SetBounds(const glm::vec2& min, const glm::vec2& max) {
    m_Min = min;
    m_Max = max;
}

template <typename T>
bool SpatialGrid<T>::IsInsideBounds(const glm::vec2& pos) const {
    return (pos.x >= m_Min.x && pos.x <= m_Max.x && pos.y >= m_Min.y && pos.y <= m_Max.y);
}

template <typename T>
std::vector<T> SpatialGrid<T>::GetAllItems() const {
    std::vector<T> all;
    all.reserve(m_ItemToCell.size());

    for (const auto& [cell, list] : m_Grid) {
        all.insert(all.end(), list.begin(), list.end());
    }

    return all;
}

}  // namespace gr
