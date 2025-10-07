#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/RandomDetourMovement.hpp>

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
    float energy = agent.GetEnergyLevel();
    float speed = maxV * (0.5f + 0.5f * energy);
    if (speed > maxV) speed = maxV;

    // Box2D-Integration via Agent-Facade
    agent.SetVelocity(dir * speed);
    agent.AddActivityCost(speed, dt);
}
}  // namespace gr
