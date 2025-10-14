#include <gramma/model/particle/ParticleSystem.hpp>
#include <memory>
#include <string>

namespace gr {

ParticleSystem::ParticleSystem(int width, int height, float cellSize) : m_Grid(cellSize) {
    m_Size.x = width;
    m_Size.y = height;
}

void ParticleSystem::Init(size_t count) {
    Clear();

    std::uniform_real_distribution<float> posX(0.0f, m_Size.x);
    std::uniform_real_distribution<float> posY(0.0f, m_Size.y);
    std::uniform_int_distribution<int> grp(0, 1);  // groups

    for (size_t i = 0; i < count; ++i) {
        auto pos = glm::vec2(posX(m_Rng), posY(m_Rng));
        auto p = std::make_unique<Particle>(std::to_string(i), pos, grp(m_Rng));
        m_Particles.push_back(std::move(p));
    }
}

void ParticleSystem::SetBehavior(std::unique_ptr<ParticleBehavior> behavior) {
    m_Behavior = std::move(behavior);
}

void ParticleSystem::AddParticle(std::unique_ptr<Particle> p) {
    m_Particles.push_back(std::move(p));
}

void ParticleSystem::Clear() {
    m_Particles.clear();
    m_Grid.Clear();
}

void ParticleSystem::UpdateGrid() {
    m_Grid.Clear();
    for (const auto& p : m_Particles) {
        m_Grid.Insert(p->GetId(), p->GetPosition(), p.get());
    }
}

void ParticleSystem::Step(float dt) {
    if (!m_Behavior) return;

    UpdateGrid();
    for (auto& p : m_Particles) {
        m_Behavior->Update(*p, dt, m_Grid);
    }
}

const std::vector<std::unique_ptr<Particle>>& ParticleSystem::GetParticles() const {
    return m_Particles;
}

}  // namespace gr
