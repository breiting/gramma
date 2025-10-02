#pragma once
#include <gramma/model/Environment.hpp>
#include <gramma/model/Task.hpp>

namespace gr {

class RelocateTask : public Task {
   public:
    RelocateTask() = default;
    RelocateTask(int width, int height);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override {
        return m_Done;
    }

   private:
    bool m_Done = false;
    int m_Width;
    int m_Height;
};

}  // namespace gr
