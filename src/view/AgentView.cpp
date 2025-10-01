#include <glm/ext/matrix_transform.hpp>
#include <gramma/view/AgentView.hpp>

#include "GLFW/glfw3.h"
#include "gramma/view/Uniforms.hpp"

namespace gr {

static glm::vec4 AgeColor(AgeClass age) {
    switch (age) {
        case AgeClass::Child:
            return {1.0f, 1.0f, 0.0f, 1.0f};  // yellow
        case AgeClass::Teenager:
            return {0.0f, 1.0f, 0.0f, 1.0f};  // green
        case AgeClass::Adult:
            return {0.0f, 0.5f, 1.0f, 1.0f};  // blue
        case AgeClass::Senior:
            return {1.0f, 0.0f, 0.0f, 1.0f};  // red
    }
    return {1.0f, 1.0f, 1.0f, 1.0f};  // fallback white
}

void AgentView::Init() {
    m_Shader.BuildAgent();
}

void AgentView::Draw(const Agent& agent, const glm::mat4& vp) {
    glm::vec2 pos = agent.Position;
    const auto& traits = agent.Traits;

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);
    m_Shader.SetVec2(Uniforms::POSITION, pos);
    m_Shader.SetFloat(Uniforms::INNER_RADIUS, traits.bodyRadius);
    m_Shader.SetFloat(Uniforms::OUTER_RADIUS, traits.comfortRadius);
    m_Shader.SetVec4(Uniforms::COLOR, AgeColor(traits.age));
    m_Shader.SetFloat(Uniforms::TIME, glfwGetTime());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}  // namespace gr
