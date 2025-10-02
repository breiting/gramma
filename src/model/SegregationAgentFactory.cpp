#include <cstdlib>
#include <gramma/model/SegregationAgentFactory.hpp>

namespace gr {

std::unique_ptr<Agent> SegregationAgentFactory::Create(Environment* env) {
    SegregationTraits traits = RandomTraits();
    glm::vec2 pos = RandomPosition(env->GetWidth(), env->GetHeight());
    float heading = RandomHeading();

    auto agent = std::make_unique<Agent>(pos, heading, traits);

    // Only SatisfactionNeed
    agent->AddNeed(std::make_unique<SatisfactionNeed>(1.0f));

    return agent;
}

SegregationTraits SegregationAgentFactory::RandomTraits() {
    SegregationTraits traits;
    traits.group = (rand() % 2 == 0 ? Group::Red : Group::Blue);
    traits.minSameFraction = 0.3f;  // tolerance
    traits.bodyRadius = 0.2f;
    traits.comfortRadius = 1.0f;
    traits.speedPref = 0.0f;  // not used here
    return traits;
}

}  // namespace gr
