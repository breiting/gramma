#pragma once
#include <gramma/model/Agent.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

class AgentView {
   public:
    AgentView() = default;
    ~AgentView();

    void Init();
    void Draw(Agent* agent, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_Vao;
};

}  // namespace gr
