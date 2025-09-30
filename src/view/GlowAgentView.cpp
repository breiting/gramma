#include <glm/ext/matrix_transform.hpp>
#include <gramma/view/GlowAgentView.hpp>

#include "GLFW/glfw3.h"
#include "gramma/view/Uniforms.hpp"

namespace gr {

static glm::vec4 AgeColor(AgeClass age) {
    switch (age) {
        case AgeClass::Child:
            return {1.0f, 1.0f, 0.0f, 1.0f};  // yellow
        case AgeClass::Young:
            return {0.0f, 1.0f, 0.0f, 1.0f};  // green
        case AgeClass::Adult:
            return {0.0f, 0.5f, 1.0f, 1.0f};  // blue
        case AgeClass::Senior:
            return {1.0f, 0.0f, 0.0f, 1.0f};  // red
    }
    return {1.0f, 1.0f, 1.0f, 1.0f};  // fallback white
}

void GlowAgentView::Init() {
    m_BodyShader.BuildBasic();
    m_RingShader.BuildRing();

    m_Body.Init();
    m_Ring.Init();
}

void GlowAgentView::Draw(const Agent& agent, const glm::mat4& vp) {
    glm::vec2 pos = agent.Pos;
    const auto& traits = agent.traits;

    auto translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
    auto mvp = vp * translation;

    // Body
    {
        m_Body.Set(pos, traits.bodyRadius);
        m_Body.SetFilled(true);
        glm::vec4 color = AgeColor(traits.age);
        m_Body.Draw(m_BodyShader, vp, color);
    }

    // Social
    {
        m_Ring.Set({0.0, 0.0}, 1.0);
        glm::vec4 color = AgeColor(traits.age);
        m_RingShader.Bind();
        m_RingShader.SetFloat("uRadius", traits.comfortRadius);
        m_RingShader.SetFloat("uThickness", traits.comfortRadius * 0.3);
        m_RingShader.SetFloat("uGlow", traits.comfortRadius * 0.1);
        m_RingShader.SetFloat("uTime", glfwGetTime());

        m_Ring.SetFilled(false);  // Outline mode
        m_Ring.Draw(m_RingShader, mvp, color);
    }
}

}  // namespace gr
