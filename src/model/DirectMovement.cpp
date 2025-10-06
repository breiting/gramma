#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/DirectMovement.hpp>

namespace gr {

void DirectMovement::Update(Agent& agent, const glm::vec2& target, float dt) {
    glm::vec2 to = target - agent.GetPosition();
    float dist = glm::length(to);
    if (dist <= 1e-4f) {
        agent.SetVelocity({0, 0});
        return;
    }

    glm::vec2 dir = to / dist;

    float maxV = agent.GetTraits().maxSpeed;

    // Velocity scales with energy
    float energy = agent.GetEnergyLevel();
    float speed = maxV * (0.5f + 0.5f * energy);
    if (speed > maxV) speed = maxV;

    agent.SetVelocity(dir * speed);
    agent.SetPosition(agent.GetPosition() + agent.GetVelocity() * dt);

    agent.AddActivityCost(glm::length(agent.GetVelocity()), dt);
}

}  // namespace gr
