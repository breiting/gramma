#include <gramma/model/RandomWalkTask.hpp>

namespace gr {

RandomWalkTask::RandomWalkTask(float duration) : m_Duration(duration) {
}

void RandomWalkTask::Start(Agent& agent) {
    float angleDeg = static_cast<float>(rand() % 360);
    agent.Heading = angleDeg;

    float angleRad = glm::radians(angleDeg);
    agent.Velocity = glm::vec2(sin(angleRad), cos(angleRad)) * agent.Traits.speedPref;
}

void RandomWalkTask::Update(Agent& agent, float dt) {
    m_Elapsed += dt;
    if (m_Elapsed > m_Duration) return;

    agent.Position += agent.Velocity * dt;
}

bool RandomWalkTask::IsFinished() const {
    return m_Elapsed > m_Duration;
}

}  // namespace gr
