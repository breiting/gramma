#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/DirectMovement.hpp>

namespace gr {

void DirectMovement::Update(Agent& agent, const glm::vec2& target, float dt) {
    glm::vec2 dir = target - agent.GetPosition();
    float dist = glm::length(dir);

    if (dist > 1e-3f) {
        dir /= dist;
        agent.SetVelocity(dir * agent.GetTraits()->maxSpeed);
        agent.SetPosition(agent.GetPosition() + agent.GetVelocity() * dt);
    } else {
        agent.SetVelocity({0.0f, 0.0f});
    }
}

}  // namespace gr
