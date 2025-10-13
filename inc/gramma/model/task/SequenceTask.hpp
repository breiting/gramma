#pragma once

#include <gramma/model/physics/IPhysicsEngine.hpp>
#include <gramma/model/task/ITask.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * Executes a sequence of tasks in order.
 * Each task is started, updated and completed before moving to the next.
 */
class SequenceTask : public ITask {
   public:
    SequenceTask() = default;

    explicit SequenceTask(std::vector<std::unique_ptr<ITask>> tasks);
    SequenceTask(std::unique_ptr<ITask> first, std::unique_ptr<ITask> second);

    void Start(Agent& agent) override;
    void Update(IPhysicsEngine* engine, Agent& agent, float dt) override;
    bool IsFinished() const override;

    void Add(std::unique_ptr<ITask> task);

   private:
    std::vector<std::unique_ptr<ITask>> m_Tasks;
    std::size_t m_CurrentIndex{0};
    bool m_Started{false};
};

}  // namespace gr
