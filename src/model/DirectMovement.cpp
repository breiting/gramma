#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/movement/DirectMovement.hpp>

namespace gr {

void DirectMovement::Update(Agent& agent, const glm::vec2& target, float dt) {
    glm::vec2 to = target - agent.GetPosition();
    float dist = glm::length(to);

    glm::vec2 dir = dist > 1e-4f ? to / dist : glm::vec2(0, 0);

    float maxV = agent.GetTraits().maxSpeed;
    float speed = maxV;

    // Fortschritt checken
    if (dist > m_LastDist - 0.01f) {  // kaum Fortschritt
        m_BlockTimer += dt;
    } else {
        m_BlockTimer = 0.0f;
    }
    m_LastDist = dist;

    if (m_BlockTimer > 0.001f) {
        float angle = glm::linearRand(-0.5f, 0.5f);  // ±30°
        float base = std::atan2(dir.y, dir.x);
        float newAngle = base + angle;
        dir = {std::cos(newAngle), std::sin(newAngle)};
        m_BlockTimer = 0.0f;
    }

    agent.SetVelocity(dir * speed);
}

}  // namespace gr
