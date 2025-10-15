#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <memory>
#include <random>
#include <vector>

namespace gr {

/**
 * @brief Factory responsible for creating particles during system initialization.
 */
class IParticleSeeder {
   public:
    virtual ~IParticleSeeder() = default;

    /**
     * @brief Produce a particle for the given index.
     * @param index Sequential index inside the initialization loop.
     * @param radius Value passed to ParticleSystem::Init.
     * @param groupCount Range of valid group identifiers [0, groupCount).
     * @param rng Shared RNG instance owned by the particle system.
     * @param minBounds Lower inclusive bounds of the simulation domain.
     * @param maxBounds Upper inclusive bounds of the simulation domain.
     */
    virtual std::unique_ptr<Particle> Create(std::size_t index, float radius, int groupCount, std::mt19937& rng,
                                             const glm::vec2& minBounds, const glm::vec2& maxBounds) = 0;
};

/**
 * @brief Storage and update loop for particle-based simulations.
 *
 * The system maintains spatial indexing, delegates motion updates to an injected
 * behavior, and exposes a pluggable seeder used to populate initial particles.
 */
class ParticleSystem {
   public:
    explicit ParticleSystem(int width, int height, float cellSize = 1.0f);

    /**
     * @brief Reset the system and populate particles using the current seeder.
     * @param count Number of particles to generate.
     * @param radius Particle radius used for initialization and default seeding.
     * @param groups Number of particle groups produced by the seeder.
     * @throws std::invalid_argument if count is zero, radius non-positive, or groups < 1.
     * @throws std::runtime_error if no seeder is configured.
     */
    void Init(size_t count, float radius, int groups);

    /**
     * @brief Override the particle seeder used by Init.
     */
    void SetSeeder(std::unique_ptr<IParticleSeeder> seeder);

    /**
     * @brief Insert a particle created externally.
     * @throws std::invalid_argument when the pointer is null.
     * @throws std::out_of_range if the particle group exceeds the configured range.
     */
    void AddParticle(std::unique_ptr<Particle> p);
    /** @brief Remove all particles while keeping the current bounds and seeder. */
    void Clear();
    /** @brief Advance every particle by one simulation step. */
    void Step(float dt);

    /**
     * @brief Install the behavior used to update particle velocities.
     * @note The caller must ensure the behavior understands the configured group count.
     */
    void SetBehavior(std::unique_ptr<IParticleBehavior> behavior);

    /** @return Immutable reference to the internal particle storage. */
    const std::vector<std::unique_ptr<Particle>>& GetParticles() const;

    void RandomizePositions();

   private:
    void UpdateGrid();

   private:
    std::vector<glm::vec2> m_Border;
    float m_XMin, m_XMax, m_YMin, m_YMax;
    std::vector<std::unique_ptr<Particle>> m_Particles;
    std::unique_ptr<IParticleBehavior> m_Behavior;
    std::unique_ptr<IParticleSeeder> m_Seeder;
    SpatialGrid<Particle*> m_Grid;
    glm::ivec2 m_Size;
    std::mt19937 m_Rng;
    int m_GroupCount = 0;
};

}  // namespace gr
