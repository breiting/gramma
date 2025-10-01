#include <gramma/model/Agent.hpp>

namespace gr {

void Agent::AssignTask(TaskPtr task) {
    m_CurrentTask = std::move(task);
    State = AgentState::Executing;
    if (m_CurrentTask) {
        m_CurrentTask->Start(*this);
    }
}

void Agent::Update(float dt, const Room* room) {
    if (!room) return;
    for (auto& s : m_Sensors) {
        s->Update(*this, *room);
    }
    if (State == AgentState::Executing && m_CurrentTask) {
        m_CurrentTask->Update(*this, dt);
        if (m_CurrentTask->IsFinished()) {
            State = AgentState::Idle;
            m_CurrentTask.reset();
        }
    }
}

void Agent::AttachSensor(SensorPtr sensor) {
    m_Sensors.push_back(std::move(sensor));
}

const std::vector<SensorPtr>& Agent::GetSensors() const {
    return m_Sensors;
}

}  // namespace gr
