#include <gramma/model/agent/SimAgentFactory.hpp>
#include <gramma/model/need/EnergyNeed.hpp>
#include <gramma/model/task/EnergyNeedTaskBuilder.hpp>
#include <gramma/model/task/SafetyNeedTaskBuilder.hpp>
#include <gramma/model/task/TaskFactory.hpp>
#include <memory>

namespace gr {

std::unique_ptr<Agent> SimAgentFactory::CreateAgent(const std::string& id, const glm::vec2& pos) {
    auto traits = std::make_unique<AgentTraits>(RandomTraits());

    auto heading = glm::vec2{0, 1};
    auto agent = std::make_unique<Agent>(id, pos, heading, std::move(traits));

    return agent;
}

void SimAgentFactory::InitTaskFactory() {
    auto& factory = gr::TaskFactory::Instance();

    factory.RegisterBuilder("Energy", std::make_shared<gr::EnergyNeedTaskBuilder>());
    factory.RegisterBuilder("Safety", std::make_shared<gr::SafetyNeedTaskBuilder>());
}

AgentTraits SimAgentFactory::RandomTraits() {
    std::uniform_int_distribution<int> ageDist(0, 3);
    std::uniform_real_distribution<float> radiusDist(0.15f, 0.25f);
    std::uniform_real_distribution<float> comfortDist(0.26f, 0.5f);
    std::uniform_real_distribution<float> speedDist(0.8f, 4.8f);
    std::uniform_real_distribution<float> mass(40, 120);  // TODO: make dependent on sex, age, ...

    AgentTraits traits;
    traits.age = static_cast<AgeClass>(ageDist(m_Rng));
    traits.bodyRadius = radiusDist(m_Rng);
    traits.socialRadius = comfortDist(m_Rng);
    traits.maxSpeed = speedDist(m_Rng);
    traits.mass = mass(m_Rng);
    return traits;
}

}  // namespace gr
