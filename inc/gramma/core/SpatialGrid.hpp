#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <unordered_map>
#include <vector>

namespace gr {

struct CellHasher {
    std::size_t operator()(const glm::ivec2& v) const noexcept {
        // Hash function for grid indexing
        return std::hash<int>()(v.x * 73856093) ^ std::hash<int>()(v.y * 19349663);
    }
};

/**
 * SpatialGrid
 * Uniform grid for fast spatial neighborhood queries (O(1) average access)
 * Scales to millions of agents/particles efficiently
 */
template <typename T>
class SpatialGrid {
   public:
    explicit SpatialGrid(float cellSize) : m_CellSize(cellSize) {
    }

    void Clear() {
        m_Grid.clear();
        m_ItemToCell.clear();
    }

    void Insert(const std::string& id, const glm::vec2& pos, T item) {
        glm::ivec2 cell = GetCellIndex(pos);
        m_Grid[cell].push_back(item);
        m_ItemToCell[id] = cell;
    }

    void Remove(const std::string& id, const glm::vec2& pos) {
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

    std::vector<const T> QueryNeighborhood(const glm::vec2& pos, float radius) const {
        std::vector<const T> result;

        int r = static_cast<int>(std::ceil(radius / m_CellSize));
        glm::ivec2 center = GetCellIndex(pos);
        float radiusSq = radius * radius;

        for (int dy = -r; dy <= r; ++dy) {
            for (int dx = -r; dx <= r; ++dx) {
                glm::ivec2 neighbor = center + glm::ivec2(dx, dy);
                auto it = m_Grid.find(neighbor);
                if (it == m_Grid.end()) continue;

                for (T const item : it->second) {
                    const glm::vec2& apos = item->GetPosition();
                    if (glm::distance2(apos, pos) <= radiusSq) {
                        result.push_back(item);
                    }
                }
            }
        }

        return result;
    }

    void SetBounds(const glm::vec2& min, const glm::vec2& max) {
        m_Min = min;
        m_Max = max;
    }

    bool IsInsideBounds(const glm::vec2& pos) const {
        return (pos.x >= m_Min.x && pos.x <= m_Max.x && pos.y >= m_Min.y && pos.y <= m_Max.y);
    }

    const glm::vec2& GetMinBounds() const {
        return m_Min;
    }
    const glm::vec2& GetMaxBounds() const {
        return m_Max;
    }

    std::vector<T> GetAllItems() const {
        std::vector<T> all;
        all.reserve(m_ItemToCell.size());

        for (const auto& [cell, list] : m_Grid) {
            all.insert(all.end(), list.begin(), list.end());
        }

        return all;
    }

   private:
    glm::ivec2 GetCellIndex(const glm::vec2& pos) const {
        return glm::ivec2(static_cast<int>(std::floor(pos.x / m_CellSize)),
                          static_cast<int>(std::floor(pos.y / m_CellSize)));
    }

   private:
    float m_CellSize;
    glm::vec2 m_Min{-1000.f};
    glm::vec2 m_Max{1000.f};

    // Grid mapping cell -> list of agent pointers
    std::unordered_map<glm::ivec2, std::vector<T>, CellHasher> m_Grid;

    // Optional: store IDs if needed
    std::unordered_map<std::string, glm::ivec2> m_ItemToCell;
};

}  // namespace gr
