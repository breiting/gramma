#include <cstdlib>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <memory>
#include <string>

namespace gr {

ParticleSystem::ParticleSystem(int width, int height, float cellSize) : m_Grid(cellSize) {
    m_Size.x = width;
    m_Size.y = height;

    m_XMin = -width / 2.0;
    m_XMax = width / 2.0;
    m_YMin = -height / 2.0;
    m_YMax = height / 2.0;

    m_Border = {{m_XMin, m_YMin}, {m_XMax, m_YMin}, {m_XMax, m_YMax}, {m_XMin, m_YMax}};
    m_Grid.SetBounds({m_XMin, m_YMin}, {m_XMax, m_YMax});
}

void ParticleSystem::Init(size_t count, float radius, int groups) {
    Clear();

    std::uniform_real_distribution<float> posX(m_XMin, m_XMax);
    std::uniform_real_distribution<float> posY(m_YMin, m_YMax);
    std::uniform_real_distribution<float> velo(0, glm::two_pi<float>());
    std::uniform_int_distribution<int> grp(0, groups - 1);

    for (size_t i = 0; i < count; ++i) {
        auto pos = glm::vec2(posX(m_Rng), posY(m_Rng));
        // auto pos = glm::vec2(0, 0);
        auto p = std::make_unique<Particle>(std::to_string(i), pos, grp(m_Rng), radius);
        float angle = velo(m_Rng);
        glm::vec2 v = glm::vec2(glm::cos(angle), glm::sin(angle)) * 1.0f;
        p->SetVelocity(v);
        m_Particles.push_back(std::move(p));
    }
}

void ParticleSystem::SetBehavior(std::unique_ptr<IParticleBehavior> behavior) {
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

    for (auto& p : m_Particles) {
        m_Behavior->Update(*p, dt, m_Grid);
        // Euler integration
        p->SetPosition(p->GetPosition() + p->GetVelocity() * dt);
    }
    UpdateGrid();
}

const std::vector<std::unique_ptr<Particle>>& ParticleSystem::GetParticles() const {
    return m_Particles;
}

}  // namespace gr
