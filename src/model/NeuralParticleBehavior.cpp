#include "gramma/model/particle/NeuralParticleBehavior.hpp"

#include <glm/glm.hpp>
#include <gramma/model/particle/Particle.hpp>

namespace gr {

NeuralParticleBehavior::NeuralParticleBehavior(float radius) : m_Radius(radius) {
}

void NeuralParticleBehavior::Update(Particle& self, float dt, const SpatialGrid<Particle*>&) const {
    auto g = self.GetGenome();
    if (!g) return;

    glm::vec2 pos = self.GetPosition();
    glm::vec2 vel = self.GetVelocity();

    std::vector<float> input = {pos.x / m_Radius, pos.y / m_Radius};
    auto out = g->Forward(input);

    glm::vec2 dir(out[0], out[1]);
    vel += dir * dt * 10.0f;
    vel *= 0.98f;
    self.SetVelocity(vel);
}

}  // namespace gr
