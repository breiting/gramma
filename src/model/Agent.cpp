#include <gramma/model/Agent.hpp>

namespace gr {

void Agent::AssignTask(TaskPtr task) {
    m_CurrentTask = std::move(task);
    State = AgentState::Executing;
    if (m_CurrentTask) {
        m_CurrentTask->Start(*this);
    }
}

void Agent::Update(float dt) {
    if (State == AgentState::Executing && m_CurrentTask) {
        m_CurrentTask->Update(*this, dt);
        if (m_CurrentTask->IsFinished()) {
            State = AgentState::Idle;
            m_CurrentTask.reset();
        }
    }
}
}  // namespace gr
