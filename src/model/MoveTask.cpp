#include <glm/glm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/task/MoveTask.hpp>

namespace gr {

MoveTask::MoveTask(const glm::vec2& target, std::unique_ptr<IMovementStrategy> strategy)
    : m_Target(target), m_Strategy(std::move(strategy)) {
}

void MoveTask::Start(Agent& /*agent*/) {
    m_Done = false;
}

void MoveTask::Update(IPhysicsEngine* engine, Agent& agent, float dt) {
    if (!engine) return;
    if (m_Done) return;
    m_Strategy->Update(engine, agent, m_Target, dt);

    float dist = glm::length(agent.GetPosition() - m_Target);
    if (dist < agent.GetTraits().bodyRadius * 0.5f) {
        engine->ApplyMovement(agent, {0, 0});
        m_Done = true;
    }
}

}  // namespace gr
