#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/RandomDetourMovement.hpp>

namespace gr {

void RandomDetourMovement::Update(Agent& agent, const glm::vec2& target, float dt) {
    glm::vec2 dir = target - agent.GetPosition();
    float dist = glm::length(dir);

    if (dist > 1e-3f) {
        dir /= dist;

        // Add small random offset
        glm::vec2 noise = glm::linearRand(glm::vec2(-0.2f, -0.2f), glm::vec2(0.2f, 0.2f));
        dir = glm::normalize(dir + noise);

        agent.SetVelocity(dir * agent.GetTraits()->maxSpeed);
        agent.SetPosition(agent.GetPosition() + agent.GetVelocity() * dt);
    } else {
        agent.SetVelocity({0.0f, 0.0f});
    }
}

}  // namespace gr
