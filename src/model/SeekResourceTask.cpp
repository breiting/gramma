#include <glm/glm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/movement/DirectMovement.hpp>
#include <gramma/model/task/SeekResourceTask.hpp>

namespace gr {

SeekResourceTask::SeekResourceTask(std::shared_ptr<IResource> resource, std::unique_ptr<IMovementStrategy> movement,
                                   float arrivalThreshold)
    : m_Res(std::move(resource)), m_Movement(std::move(movement)), m_Threshold(arrivalThreshold) {
}

void SeekResourceTask::Start(Agent& agent) {
    if (!m_Res) {
        m_Done = true;
        return;
    }

    glm::vec2 target = m_Res->GetPosition();
    if (!m_Movement) {
        // Fallback
        m_Movement = std::make_unique<DirectMovement>();
    }

    m_MoveTask = std::make_unique<MoveTask>(target, std::move(m_Movement));
    m_MoveTask->Start(agent);
}

void SeekResourceTask::Update(Agent& agent, float dt) {
    if (m_Done || !m_Res || !m_MoveTask) {
        m_Done = true;
        return;
    }

    m_MoveTask->Update(agent, dt);

    float dist = glm::length(agent.GetPosition() - m_Res->GetPosition());
    if (dist < m_Threshold) {
        m_AtRes = true;
        m_Done = true;
    }
}

bool SeekResourceTask::IsFinished() const {
    return m_Done;
}

bool SeekResourceTask::HasArrived() const {
    return m_AtRes;
}

const std::shared_ptr<IResource>& SeekResourceTask::GetResource() const {
    return m_Res;
}

}  // namespace gr
