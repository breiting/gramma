#pragma once
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/need/INeed.hpp>
#include <gramma/model/task/ITask.hpp>
#include <memory>

namespace gr {

/**
 * Interface for building a task based on a given need and agent context.
 * Implements Strategy Pattern.
 */
class INeedTaskBuilder {
   public:
    virtual ~INeedTaskBuilder() = default;

    /// Creates a task based on the agent's need and the current environment.
    virtual std::unique_ptr<ITask> Build(const INeed& need, Agent& agent, const Environment& env) const = 0;
};

}  // namespace gr
