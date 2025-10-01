#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/FoodSource.hpp>
#include <gramma/model/SeekFoodTask.hpp>

namespace gr {

SeekFoodTask::SeekFoodTask(const FoodSource* target) : m_Target(target) {
}

void SeekFoodTask::Start(Agent& agent) {
    if (!m_Target) {
        m_Finished = true;
        return;
    }
    // Agent schaut Richtung Futter
    glm::vec2 dir = m_Target->GetPosition() - agent.GetPosition();
    float angle = glm::degrees(std::atan2(dir.x, dir.y));  // 0=north
    agent.SetHeading(angle);
    agent.SetDesiredSpeed(agent.GetTraits().speedPref);
}

void SeekFoodTask::Update(Agent& agent, float dt) {
    if (!m_Target) {
        m_Finished = true;
        return;
    }

    glm::vec2 dir = m_Target->GetPosition() - agent.GetPosition();
    float dist = glm::length(dir);

    if (dist < agent.GetTraits().bodyRadius * 2.0f) {
        // "Essen erreicht"
        m_Finished = true;
        return;
    }

    dir = glm::normalize(dir);
    float angle = glm::degrees(std::atan2(dir.x, dir.y));
    agent.SetHeading(angle);
    agent.SetDesiredSpeed(agent.GetTraits().speedPref);

    // Bewegung passiert über Task selbst
    glm::vec2 velocity = dir * agent.GetDesiredSpeed();
    agent.SetPosition(agent.GetPosition() + velocity * dt);
}

bool SeekFoodTask::IsFinished() const {
    return m_Finished;
}

}  // namespace gr
