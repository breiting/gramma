#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/DirectMovement.hpp>
#include <gramma/model/FoodSource.hpp>
#include <gramma/model/SeekFoodTask.hpp>

namespace gr {

SeekFoodTask::SeekFoodTask(std::shared_ptr<FoodSource> food) : m_Food(food) {
}

void SeekFoodTask::Start(Agent& agent) {
    if (!m_Food) {
        m_Finished = true;
        return;
    }

    m_Move = std::make_unique<MoveTask>(m_Food->GetPosition(), std::make_unique<DirectMovement>());
    m_Move->Start(agent);
}

void SeekFoodTask::Update(Agent& agent, float dt) {
    if (!m_Food) {
        m_Finished = true;
        return;
    }

    m_Move->Update(agent, dt);

    // Check if close enough to eat
    float dist = glm::length(agent.GetPosition() - m_Food->GetPosition());
    if (dist < agent.GetTraits()->bodyRadius) {
        float consumed = m_Food->Consume(0.3f);  // 30% per bite
        if (consumed > 0.0f) {
            agent.SatisfyNeed("Hunger");
        }
    }
    m_Finished = m_Move->IsFinished();
}

bool SeekFoodTask::IsFinished() const {
    return m_Finished;
}

}  // namespace gr
