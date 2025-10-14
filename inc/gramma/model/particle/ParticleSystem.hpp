#pragma once

#include <glm/glm.hpp>
#include <gramma/core/SpatialGrid.hpp>
#include <gramma/model/particle/IParticleBehavior.hpp>
#include <gramma/model/particle/Particle.hpp>
#include <memory>
#include <random>
#include <vector>

namespace gr {

class ParticleSystem {
   public:
    explicit ParticleSystem(int width, int height, float cellSize = 1.0f);

    void Init(size_t count, float radius, int groups);
    void AddParticle(std::unique_ptr<Particle> p);
    void Clear();
    void Step(float dt);

    void SetBehavior(std::unique_ptr<IParticleBehavior> behavior);

    const std::vector<std::unique_ptr<Particle>>& GetParticles() const;

   private:
    void UpdateGrid();

   private:
    std::vector<glm::vec2> m_Border;
    float m_XMin, m_XMax, m_YMin, m_YMax;
    std::vector<std::unique_ptr<Particle>> m_Particles;
    std::unique_ptr<IParticleBehavior> m_Behavior;
    SpatialGrid<Particle*> m_Grid;
    glm::ivec2 m_Size;
    std::mt19937 m_Rng;
};

}  // namespace gr
