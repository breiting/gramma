#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/DirectMovement.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/MoveTask.hpp>
#include <gramma/model/SeekResourceTask.hpp>

namespace gr {

SeekResourceTask::SeekResourceTask(std::shared_ptr<IResource> res, float intakePerSec, float target)
    : m_Res(std::move(res)), m_IntakePerSec(intakePerSec), m_Target(target) {
}

void SeekResourceTask::Start(Agent& agent) {
    if (!m_Res) {
        m_Done = true;
        return;
    }
    auto move = std::make_unique<MoveTask>(m_Res->GetPosition(), std::make_unique<DirectMovement>());
    move->Start(agent);
    // Wir nutzen MoveTask „inline“: einfache Komposition
    // Alternative: Member m_Move; hier halten wir's minimal.
}

void SeekResourceTask::Update(Agent& agent, float dt) {
    if (m_Done || !m_Res) {
        m_Done = true;
        return;
    }

    // Schritt: laufen (direkt – minimal)
    DirectMovement mover;
    mover.Update(agent, m_Res->GetPosition(), dt);

    // In Reichweite?
    float dist = glm::length(m_Res->GetPosition() - agent.GetPosition());
    if (dist < agent.GetTraits().bodyRadius * 0.75f) m_AtRes = true;

    if (m_AtRes) {
        float want = m_IntakePerSec * dt;
        float got = m_Res->Consume(want);
        agent.AddEnergyIntake(got);

        if (agent.GetEnergyLevel() >= m_Target || m_Res->IsDepleted()) {
            m_Done = true;
        }
    }
}

bool SeekResourceTask::IsFinished() const {
    return m_Done;
}

}  // namespace gr
