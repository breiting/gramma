#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/view/Shader.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * AgentView:
 * All Agents with one draw call
 */
class AgentView {
   public:
    AgentView();
    ~AgentView();

    void Init();
    void UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents);
    void Draw(const glm::mat4& vp);

   private:
    struct InstanceData {
        glm::vec2 position;
        float radius;
        glm::vec4 color;
        glm::vec4 glowColor;
        float glowWidth;
        glm::vec2 heading;
    };

    GLuint m_VaoBody = 0;
    GLuint m_VaoLine = 0;
    GLuint m_InstanceVbo = 0;

    Shader m_ShaderBody;
    Shader m_ShaderLine;

    std::vector<InstanceData> m_InstanceData;
};

}  // namespace gr
