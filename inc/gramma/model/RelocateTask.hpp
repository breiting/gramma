#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Task.hpp>

namespace gr {

class RelocateTask : public Task {
   public:
    explicit RelocateTask(glm::vec2 targetPos);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override {
        return m_Done;
    }

   private:
    glm::vec2 m_Target;
    bool m_Done = false;
};

}  // namespace gr
