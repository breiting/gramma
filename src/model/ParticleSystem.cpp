#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include <gramma/model/particle/ParticleSystem.hpp>

namespace gr {

namespace {

class RandomParticleSeeder final : public IParticleSeeder {
   public:
    std::unique_ptr<Particle> Create(std::size_t index, float radius, int groupCount, std::mt19937& rng,
                                     const glm::vec2& minBounds, const glm::vec2& maxBounds) override {
        if (groupCount < 1) {
            throw std::invalid_argument("RandomParticleSeeder requires at least one group.");
        }

        std::uniform_real_distribution<float> posX(minBounds.x, maxBounds.x);
        std::uniform_real_distribution<float> posY(minBounds.y, maxBounds.y);
        std::uniform_real_distribution<float> angleDist(0.0f, glm::two_pi<float>());
        std::uniform_int_distribution<int> groupDist(0, groupCount - 1);

        const glm::vec2 position(posX(rng), posY(rng));
        auto particle =
            std::make_unique<Particle>(std::to_string(index), position, groupDist(rng), radius);

        const float angle = angleDist(rng);
        const glm::vec2 velocity = glm::vec2(glm::cos(angle), glm::sin(angle));
        particle->SetVelocity(velocity);

        return particle;
    }
};

}  // namespace

ParticleSystem::ParticleSystem(int width, int height, float cellSize) : m_Grid(cellSize) {
    m_Size.x = width;
    m_Size.y = height;

    m_XMin = -width / 2.0;
    m_XMax = width / 2.0;
    m_YMin = -height / 2.0;
    m_YMax = height / 2.0;

    m_Border = {{m_XMin, m_YMin}, {m_XMax, m_YMin}, {m_XMax, m_YMax}, {m_XMin, m_YMax}};
    m_Grid.SetBounds({m_XMin, m_YMin}, {m_XMax, m_YMax});
    m_Seeder = std::make_unique<RandomParticleSeeder>();
}

void ParticleSystem::Init(size_t count, float radius, int groups) {
    if (count == 0) {
        throw std::invalid_argument("ParticleSystem::Init requires count > 0.");
    }
    if (radius <= 0.0f) {
        throw std::invalid_argument("ParticleSystem::Init requires radius > 0.");
    }
    if (groups < 1) {
        throw std::invalid_argument("ParticleSystem::Init requires at least one group.");
    }
    if (!m_Seeder) {
        throw std::runtime_error("ParticleSystem::Init requires a particle seeder.");
    }

    Clear();

    m_GroupCount = groups;
    for (size_t i = 0; i < count; ++i) {
        auto p = m_Seeder->Create(i, radius, groups, m_Rng, {m_XMin, m_YMin}, {m_XMax, m_YMax});
        if (!p) {
            throw std::runtime_error("ParticleSystem seeder returned a null particle.");
        }
        const int group = p->GetGroup();
        if (group < 0 || group >= groups) {
            throw std::out_of_range("ParticleSystem seeder produced a particle with an invalid group id.");
        }
        m_Particles.push_back(std::move(p));
    }
}

void ParticleSystem::SetSeeder(std::unique_ptr<IParticleSeeder> seeder) {
    m_Seeder = std::move(seeder);
}

void ParticleSystem::SetBehavior(std::unique_ptr<IParticleBehavior> behavior) {
    m_Behavior = std::move(behavior);
}

void ParticleSystem::AddParticle(std::unique_ptr<Particle> p) {
    if (!p) {
        throw std::invalid_argument("ParticleSystem::AddParticle received a null particle.");
    }
    if (m_GroupCount > 0) {
        const int group = p->GetGroup();
        if (group < 0 || group >= m_GroupCount) {
            throw std::out_of_range("ParticleSystem::AddParticle received a particle with an invalid group id.");
        }
    }
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

    tbb::parallel_for(tbb::blocked_range<size_t>(0, m_Particles.size()), [&](const tbb::blocked_range<size_t>& range) {
        for (size_t i = range.begin(); i < range.end(); ++i) {
            Particle* p = m_Particles[i].get();
            m_Behavior->Update(*p, dt, m_Grid);
        }
    });

    for (auto& p : m_Particles) {
        p->SetPosition(p->GetPosition() + p->GetVelocity() * dt);
    }
}

const std::vector<std::unique_ptr<Particle>>& ParticleSystem::GetParticles() const {
    return m_Particles;
}

}  // namespace gr
