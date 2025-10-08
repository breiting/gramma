#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/DirectMovement.hpp>
#include <gramma/model/Exit.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/MoveTask.hpp>
#include <gramma/model/SeekResourceTask.hpp>

#include "gramma/model/Types.hpp"

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
}

void SeekResourceTask::Update(Agent& agent, float dt) {
    if (m_Done || !m_Res) {
        m_Done = true;
        return;
    }

    DirectMovement mover;
    mover.Update(agent, m_Res->GetPosition(), dt);

    float dist = glm::length(m_Res->GetPosition() - agent.GetPosition());
    float seekRadius = agent.GetTraits().bodyRadius * 0.75;
    if (auto* e = dynamic_cast<Exit*>(m_Res.get())) {
        seekRadius = e->GetWidth();
    }
    if (dist < seekRadius) m_AtRes = true;

    if (m_AtRes) {
        // Is it an Exit?
        if (auto* e = dynamic_cast<Exit*>(m_Res.get())) {
            m_Done = true;
            agent.SetState(AgentState::Rescued);
            return;
        }

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
