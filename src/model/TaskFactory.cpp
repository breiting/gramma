#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/movement/DirectMovement.hpp>
#include <gramma/model/need/EnergyNeed.hpp>
#include <gramma/model/need/INeed.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/MoveTask.hpp>
#include <gramma/model/task/RestTask.hpp>
#include <gramma/model/task/SeekResourceTask.hpp>
#include <gramma/model/task/TaskFactory.hpp>
#include <memory>

namespace gr {

std::unique_ptr<ITask> TaskFactory::MakeFor(const INeed& need, Agent& agent, const Environment& env) {
    if (need.Name() == "Energy") {
        // Rule: if energy is below 'low' search for resource, otherwise go home
        // const auto* e = dynamic_cast<const EnergyNeed*>(&need);
        // float level = agent.GetEnergyLevel();
        // if (e && level < e->LowThreshold()) {
        //     auto nearest = env.FindNearest(ResourceType::Food, agent.GetPosition());
        //     if (nearest) {
        //         return std::make_unique<SeekResourceTask>(nearest, e->IntakeGain(), e->Target());
        //     }
        //     // keine Ressource gefunden -> als Fallback ruhen (bringt wenig, aber blockiert nicht)
        //     return std::make_unique<RestTask>(e ? e->Target() : 0.85f);
        // } else {
        //     if (agent.GetHome()) {
        //         return std::make_unique<MoveTask>(agent.GetHome()->GetPosition(),
        //         std::make_unique<DirectMovement>());
        //     }
        // }
    } else if (need.Name() == "Walk") {
        return std::make_unique<MoveTask>(env.RandomPosition(), std::make_unique<DirectMovement>());
    } else if (need.Name() == "Safety") {
        auto nearest = env.FindNearest(ResourceType::Exit, agent.GetPosition());
        return std::make_unique<SeekResourceTask>(nearest);
    }

    // Default: nichts
    return {};
}

}  // namespace gr
