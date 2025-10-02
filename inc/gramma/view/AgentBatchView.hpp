#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/view/Shader.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * AgentBatchView:
 * All Agents with one draw call
 */
class AgentBatchView {
   public:
    AgentBatchView();
    ~AgentBatchView();

    void Init();
    void UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents);
    void Draw(const glm::mat4& vp);

   private:
    struct InstanceData {
        glm::vec2 position;
        float innerRadius;
        float outerRadius;
        float blendWidth;
        glm::vec4 color;
        glm::vec4 glowColor;
        float thickness;
        float heading;
        float fov;
    };

    Shader m_Shader;
    GLuint m_Vao = 0;
    GLuint m_InstanceVbo = 0;

    std::vector<InstanceData> m_InstanceData;
};

}  // namespace gr
