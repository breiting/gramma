#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/movement/DirectMovement.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/SafetyNeedTaskBuilder.hpp>
#include <iostream>
#include <memory>

namespace gr {

std::unique_ptr<ITask> SafetyNeedTaskBuilder::Build(const INeed& /*need*/, Agent& agent, const Environment& env) const {
    auto exit = env.FindNearest(ResourceType::Exit, agent.GetPosition());
    if (!exit) {
        std::cerr << "[SafetyNeedTaskBuilder] No exit found.\n";
        return nullptr;
    }
    auto ptr = dynamic_cast<Exit*>(exit.get());
    if (ptr->IsBlocked()) {
        return nullptr;
    }

    return std::make_unique<SeekResourceTask>(exit, std::make_unique<DirectMovement>(),
                                              exit->GetBoundingRadius() * 2.0);
}

}  // namespace gr
