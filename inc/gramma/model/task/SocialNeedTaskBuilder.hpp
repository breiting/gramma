#pragma once

#include <gramma/model/movement/IMovementStrategy.hpp>
#include <gramma/model/task/INeedTaskBuilder.hpp>
#include <gramma/model/task/SeekAgentTask.hpp>
#include <memory>

namespace gr {

/**
 * @brief Creates a SeekAgentTask based on the SocialNeed.
 */
class SocialNeedTaskBuilder : public INeedTaskBuilder {
   public:
    std::unique_ptr<ITask> Build(const INeed& need, Agent& agent, const Environment& env) const override;
};

}  // namespace gr
