#include <gramma/model/Agent.hpp>
#include <gramma/model/RestTask.hpp>

namespace gr {

void RestTask::Update(Agent& agent, float dt) {
    agent.AddEnergyRest(dt);
    m_Done = (agent.GetEnergyLevel() >= m_Target);
}

}  // namespace gr
