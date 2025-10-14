#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/task/RestTask.hpp>

#include "gramma/model/physics/IPhysicsEngine.hpp"

namespace gr {

RestTask::RestTask(float durationSeconds) : m_Duration(durationSeconds) {
}

void RestTask::Start(Agent& /*agent*/) {
    m_Elapsed = 0.0f;
    m_Done = false;
}

void RestTask::Update(IPhysicsEngine* /*engine*/, Agent& /*agent*/, float dt) {
    if (m_Done) return;

    m_Elapsed += dt;
    if (m_Elapsed >= m_Duration) {
        m_Done = true;
    }
}

bool RestTask::IsFinished() const {
    return m_Done;
}

}  // namespace gr
