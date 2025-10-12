#pragma once
#include <gramma/model/task/INeedTaskBuilder.hpp>
#include <gramma/model/task/SeekResourceTask.hpp>

namespace gr {

/**
 * Builds tasks for SafetyNeed.
 * -> Seek nearest Exit resource.
 */
class SafetyNeedTaskBuilder : public INeedTaskBuilder {
   public:
    std::unique_ptr<ITask> Build(const INeed& need, Agent& agent, const Environment& env) const override;
};

}  // namespace gr
