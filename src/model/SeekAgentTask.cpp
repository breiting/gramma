#include <glm/glm.hpp>
#include <gramma/model/task/SeekAgentTask.hpp>

#include "gramma/model/physics/IPhysicsEngine.hpp"

namespace gr {

SeekAgentTask::SeekAgentTask(const Agent* target, std::unique_ptr<IMovementStrategy> mover)
    : m_Target(target), m_Movement(std::move(mover)) {
}

void SeekAgentTask::Start(Agent& /*agent*/) {
    m_Done = false;
}

void SeekAgentTask::Update(IPhysicsEngine* engine, Agent& agent, float dt) {
    if (!m_Target) {
        m_Done = true;
        return;
    }

    // Move towards target position
    m_Movement->Update(engine, agent, m_Target->GetPosition(), dt);

    float dist = glm::length(agent.GetPosition() - m_Target->GetPosition());
    if (dist < agent.GetTraits().bodyRadius * 1.5f) {
        m_Done = true;
    }
}

bool SeekAgentTask::IsFinished() const {
    return m_Done;
}

}  // namespace gr
