#pragma once
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/movement/IMovementStrategy.hpp>
#include <gramma/model/task/ITask.hpp>
#include <memory>

namespace gr {

/**
 * @brief A task where an agent moves towards another target agent.
 * Often used for social attraction or flocking behavior.
 */
class SeekAgentTask : public ITask {
   public:
    SeekAgentTask(const Agent* target, std::unique_ptr<IMovementStrategy> mover);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    const Agent* m_Target;
    std::unique_ptr<IMovementStrategy> m_Movement;
    bool m_Done{false};
};

}  // namespace gr
