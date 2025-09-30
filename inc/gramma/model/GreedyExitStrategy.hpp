#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/model/Exit.hpp>
#include <gramma/model/INavigationStrategy.hpp>

namespace gr {

/** Navigation strategy that moves agents directly towards the exit. */
class GreedyExitStrategy : public INavigationStrategy {
   public:
    glm::vec2 ComputeDesiredVelocity(const Agent& agent, const Exit& exit) const override;
};

}  // namespace gr