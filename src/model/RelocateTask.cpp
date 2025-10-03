#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/RelocateTask.hpp>

namespace gr {

RelocateTask::RelocateTask(glm::vec2 targetPos) : m_Target(targetPos) {
}

void RelocateTask::Start(Agent& agent) {
    glm::vec2 dir = m_Target - agent.GetPosition();
    float dist = glm::length(dir);
    agent.SetSpeed(agent.GetTraits()->maxSpeed);

    if (dist > 1e-3f) {
        dir /= dist;
        agent.SetHeading(glm::degrees(std::atan2(dir.x, dir.y)));
    } else {
        m_Done = true;
    }
}

void RelocateTask::Update(Agent& agent, float dt) {
    if (m_Done) return;

    glm::vec2 pos = agent.GetPosition();
    glm::vec2 toTarget = m_Target - pos;
    float dist = glm::length(toTarget);

    if (dist < agent.GetTraits()->bodyRadius * 0.5f) {
        agent.SetPosition(m_Target);
        m_Done = true;
    } else {
        glm::vec2 dir = toTarget / dist;
        agent.SetHeading(glm::degrees(std::atan2(dir.x, dir.y)));
        agent.SetPosition(pos + agent.GetVelocity() * dt);
    }
}

}  // namespace gr
