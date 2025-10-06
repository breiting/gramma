#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/SimAgentFactory.hpp>
#include <memory>

namespace gr {

std::unique_ptr<Agent> SimAgentFactory::Create(Environment* env) {
    auto traits = std::make_unique<AgentTraits>(RandomTraits());

    glm::vec2 pos = RandomPosition(env->GetWidth(), env->GetHeight());
    float heading = RandomHeading();

    auto agent = std::make_unique<Agent>(pos, heading, std::move(traits));

    // Needs
    agent->AddNeed(std::make_unique<EnergyNeed>());

    return agent;
}

AgentTraits SimAgentFactory::RandomTraits() {
    std::uniform_int_distribution<int> ageDist(0, 3);
    std::uniform_real_distribution<float> radiusDist(0.15f, 0.25f);
    std::uniform_real_distribution<float> comfortDist(0.26f, 0.5f);
    std::uniform_real_distribution<float> speedDist(0.8f, 4.8f);

    AgentTraits traits;
    traits.age = static_cast<AgeClass>(ageDist(m_Rng));
    traits.bodyRadius = radiusDist(m_Rng);
    traits.socialRadius = comfortDist(m_Rng);
    traits.maxSpeed = speedDist(m_Rng);
    return traits;
}

}  // namespace gr
