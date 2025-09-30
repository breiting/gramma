#pragma once
#include <gramma/view/CircleShape.hpp>
#include <gramma/view/IAgentView.hpp>
#include <gramma/view/Shader.hpp>

#include "gramma/view/LineShape.hpp"

namespace gr {

class GlowAgentView : public IAgentView {
   public:
    GlowAgentView() = default;

    void Init() override;

    void Draw(const Agent& agent, const glm::mat4& vp) override;

   private:
    Shader m_BodyShader;
    Shader m_RingShader;

    CircleShape m_Body;
    CircleShape m_Ring;
};

}  // namespace gr
