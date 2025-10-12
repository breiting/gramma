#pragma once
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/INeedTaskBuilder.hpp>
#include <gramma/model/task/RestTask.hpp>
#include <gramma/model/task/SeekResourceTask.hpp>

namespace gr {

/**
 * Builds tasks for EnergyNeed.
 * -> Go to nearest Food resource, then rest.
 */
class EnergyNeedTaskBuilder : public INeedTaskBuilder {
   public:
    std::unique_ptr<ITask> Build(const INeed& need, Agent& agent, const Environment& env) const override;
};

}  // namespace gr
