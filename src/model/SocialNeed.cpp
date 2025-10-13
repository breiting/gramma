#include <glm/glm.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/need/SocialNeed.hpp>

constexpr float MAX_SURROUNDED_AGENTS = 20.0f;

namespace gr {

float SocialNeed::Evaluate(const Agent& self, const Environment& env) const {
    float density =
        static_cast<float>(env.QueryAgentsInRadius(self.GetPosition(), self.GetTraits().socialRadius).size());
    return glm::clamp(density / MAX_SURROUNDED_AGENTS, 0.0f,
                      1.0f);  // Full need if max sourrounded agents is satisfied
}

float SocialNeed::Utility(const Agent& self, const Environment& env) const {
    return Evaluate(self, env);
}

}  // namespace gr
