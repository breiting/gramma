#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/movement/RandomDetourMovement.hpp>

namespace gr {

void RandomDetourMovement::Update(Agent& agent, const glm::vec2& target, float dt) {
    glm::vec2 to = target - agent.GetPosition();
    float dist = glm::length(to);

    if (dist <= 1e-4f) {
        agent.SetVelocity({0, 0});
        return;
    }

    glm::vec2 dir = to / dist;

    // Zufallsstörung hinzufügen
    glm::vec2 noise = glm::linearRand(glm::vec2(-0.15f), glm::vec2(0.15f));
    dir = glm::normalize(dir + noise);

    // Geschwindigkeit berechnen
    float maxV = agent.GetTraits().maxSpeed;
    float speed = maxV;
    if (speed > maxV) speed = maxV;

    agent.SetVelocity(dir * speed);
}
}  // namespace gr
