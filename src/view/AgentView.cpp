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

AgentView::~AgentView() {
    if (m_Vao) {
        glDeleteVertexArrays(1, &m_Vao);
    }
}

void AgentView::Init() {
    m_Shader.BuildAgent();

    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    glBindVertexArray(0);
}

void AgentView::Draw(const Agent* agent, const glm::mat4& vp) {
    if (!agent) return;
    glm::vec2 pos = agent->GetPosition();
    const auto& traits = agent->GetTraits();

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);
    m_Shader.SetVec2(Uniforms::POSITION, pos);
    m_Shader.SetFloat(Uniforms::INNER_RADIUS, traits->bodyRadius);
    m_Shader.SetFloat(Uniforms::OUTER_RADIUS, traits->comfortRadius);
    m_Shader.SetFloat(Uniforms::BLEND_WIDTH, traits->bodyRadius * 0.1);
    m_Shader.SetVec4(Uniforms::COLOR, AgeColor(traits->age));
    m_Shader.SetVec4(Uniforms::GLOW_COLOR, glm::vec4(1.0, 0.2, 0.2, 1.0));
    m_Shader.SetFloat(Uniforms::THICKNESS, 0.1f);
    m_Shader.SetFloat(Uniforms::TIME, glfwGetTime());
    m_Shader.SetFloat(Uniforms::HEADING, glm::radians(agent->GetHeading()));
    m_Shader.SetFloat(Uniforms::FOV, 30.0);

    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

}  // namespace gr
