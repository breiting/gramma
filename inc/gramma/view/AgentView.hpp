#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

class AgentView {
   public:
    AgentView() = default;

    void Init();
    void Draw(const Agent& agent, const glm::mat4& vp);

   private:
    Shader m_Shader;
};

}  // namespace gr
