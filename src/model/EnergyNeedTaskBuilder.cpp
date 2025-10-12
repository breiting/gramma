#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/task/EnergyNeedTaskBuilder.hpp>
#include <gramma/model/task/RestTask.hpp>
#include <gramma/model/task/SeekResourceTask.hpp>
#include <gramma/model/task/SequenceTask.hpp>
#include <iostream>

namespace gr {

std::unique_ptr<ITask> EnergyNeedTaskBuilder::Build(const INeed& /*need*/, Agent& agent, const Environment& env) const {
    auto res = env.FindNearest(ResourceType::Food, agent.GetPosition());
    if (!res) {
        std::cerr << "[EnergyNeedTaskBuilder] No food resource found.\n";
        return nullptr;
    }

    auto seek = std::make_unique<SeekResourceTask>(res);
    auto rest = std::make_unique<RestTask>(2.0f);

    return std::make_unique<SequenceTask>(std::move(seek), std::move(rest));
}

}  // namespace gr
