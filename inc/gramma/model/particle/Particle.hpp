#pragma once

#include <glm/glm.hpp>
#include <string>

namespace gr {

class Genome;

/**
 * @brief Lightweight data object representing a simulated particle.
 *
 * A particle stores its identifier, spatial properties, group membership, and
 * optional traits such as radius. All coordinates are expressed in world space.
 */
class Particle {
   public:
    /**
     * @param id Unique identifier for serialization and spatial indexing.
     * @param position Initial world-space position.
     * @param group Logical group or species identifier used by behaviors.
     * @param radius Rendering and collision radius (defaults to 0.1 units).
     */
    explicit Particle(std::string id, const glm::vec2& position, int group, float radius = 0.1);

    /** @return Immutable particle identifier. */
    const std::string& GetId() const;
    /** @return Current world-space position. */
    const glm::vec2& GetPosition() const;
    /** @brief Update the particle position in world space. */
    void SetPosition(const glm::vec2& pos);

    /** @return Current velocity vector. */
    const glm::vec2& GetVelocity() const;
    /** @brief Set the instantaneous velocity vector. */
    void SetVelocity(const glm::vec2& vel);

    /** @return Rendering and interaction radius. */
    float GetRadius() const;

    /** @return Genome, nullptr if not available. */
    Genome* GetGenome();

    /** @brief Set the genome (optional). */
    void SetGenome(Genome* gen);

    /** @return Group identifier used by interaction behaviors. */
    int GetGroup() const;

   private:
    std::string m_Id;
    glm::vec2 m_Position;
    glm::vec2 m_Velocity{0.0f};
    int m_Group;
    float m_Radius;
    Genome* m_Genome{nullptr};
};

}  // namespace gr
