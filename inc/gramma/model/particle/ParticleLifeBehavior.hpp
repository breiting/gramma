#pragma once

#include <cstddef>
#include <vector>

#include <glm/glm.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>

namespace gr {

/**
 * @brief Particle interaction rule reminiscent of the Particle Life model.
 *
 * The behavior samples nearby particles inside a configurable radius and derives
 * directional forces from an attraction/repulsion matrix. Entries are addressed
 * by group id, therefore the provided matrix must be square and large enough to
 * cover every group produced by the particle system.
 */
class ParticleLifeBehavior : public IParticleBehavior {
   public:
    /**
     * @param radius Maximum interaction radius to query in the spatial grid.
     * @param matrix Square attraction/repulsion matrix indexed by particle group.
     * @throws std::invalid_argument if the matrix is empty or non-square.
     */
    ParticleLifeBehavior(float radius, const std::vector<std::vector<float>>& matrix);

    /**
     * @brief Integrate forces for a single particle based on neighborhood queries.
     * @throws std::out_of_range if either particle or neighbor report an invalid group id.
     */
    void Update(Particle& p, float dt, const SpatialGrid<Particle*>& grid) const override;

   private:
    float m_Radius;
    float m_CoreRadius;
    std::vector<std::vector<float>> m_AttractionMatrix;
    std::size_t m_GroupCount = 0;
};

}  // namespace gr
