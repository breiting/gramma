#include <glm/glm.hpp>
#include <gramma/model/movement/DirectMovement.hpp>
#include <gramma/model/movement/RandomDetourMovement.hpp>
#include <gramma/model/need/SocialNeed.hpp>
#include <gramma/model/social/SocialSensor.hpp>
#include <gramma/model/task/SocialNeedTaskBuilder.hpp>

namespace gr {

std::unique_ptr<ITask> SocialNeedTaskBuilder::Build(const INeed& need, Agent& agent, const Environment& env) const {
    if (!dynamic_cast<const SocialNeed*>(&need)) return nullptr;

    const Agent* target = SocialSensor::FindMostAttractive(agent, env);
    if (target) {
        return std::make_unique<SeekAgentTask>(target, std::make_unique<RandomDetourMovement>());
    }

    return nullptr;
}

}  // namespace gr
