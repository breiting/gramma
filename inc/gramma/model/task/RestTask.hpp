#pragma once
#include <gramma/model/task/ITask.hpp>

namespace gr {

class RestTask : public ITask {
   public:
    explicit RestTask(float target = 0.85f) : m_Target(target) {
    }

    void Start(Agent& /*agent*/) override {
    }
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override {
        return m_Done;
    }

   private:
    float m_Target;
    bool m_Done{false};
};

}  // namespace gr
