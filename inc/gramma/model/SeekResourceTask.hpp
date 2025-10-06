#pragma once
#include <gramma/model/ITask.hpp>
#include <gramma/model/Types.hpp>
#include <memory>

namespace gr {
class IResource;

class SeekResourceTask : public ITask {
   public:
    SeekResourceTask(std::shared_ptr<IResource> resource, float intakePerSec = 0.015f, float target = 0.85f);

    void Start(Agent& agent) override;
    void Update(Agent& agent, float dt) override;
    bool IsFinished() const override;

   private:
    std::shared_ptr<IResource> m_Res;
    float m_IntakePerSec;
    float m_Target;
    bool m_AtRes{false};
    bool m_Done{false};
};

}  // namespace gr
