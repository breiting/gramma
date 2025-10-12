#pragma once

#include <gramma/model/task/ITask.hpp>

namespace gr {

/**
 * A simple task that keeps the agent idle for a given duration.
 */
class RestTask : public ITask {
   public:
    explicit RestTask(float durationSeconds = 1.0f);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    float m_Duration;
    float m_Elapsed{0.0f};
    bool m_Done{false};
};

}  // namespace gr
