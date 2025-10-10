// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/Uniforms.hpp>

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
    return {1.0f, 1.0f, 1.0f, 1.0f};  // fallback
}

AgentView::AgentView() {
}
AgentView::~AgentView() {
    if (m_VaoBody) glDeleteVertexArrays(1, &m_VaoBody);
    if (m_VaoLine) glDeleteVertexArrays(1, &m_VaoLine);
    if (m_InstanceVbo) glDeleteBuffers(1, &m_InstanceVbo);
}

void AgentView::Init() {
    m_ShaderBody.BuildAgentBody();
    m_ShaderLine.BuildAgentLine();

    glGenVertexArrays(1, &m_VaoBody);
    glGenVertexArrays(1, &m_VaoLine);
    glGenBuffers(1, &m_InstanceVbo);

    // Layout (binding für Shader-Instanced Attributes)
    std::size_t stride = sizeof(InstanceData);

    // ----- Body VAO -----
    glBindVertexArray(m_VaoBody);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);

    glEnableVertexAttribArray(1);  // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);  // radius
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, radius));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);  // color
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);  // glowColor
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, glowColor));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);  // glowWidth
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, glowWidth));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);  // heading
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, heading));
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    // ----- Line VAO -----
    glBindVertexArray(m_VaoLine);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);

    glEnableVertexAttribArray(1);  // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);  // radius
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, radius));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);  // color
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);  // glowColor
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, glowColor));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);  // glowWidth
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, glowWidth));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);  // heading
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, heading));
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void AgentView::UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents) {
    m_InstanceData.clear();
    m_InstanceData.reserve(agents.size());

    for (auto& a : agents) {
        if (a->GetState() == AgentState::Dead || a->GetState() == AgentState::Rescued) continue;
        const auto& traits = a->GetTraits();

        auto color = AgeColor(traits.age);

        InstanceData d;
        d.position = a->GetPosition();
        d.radius = traits.bodyRadius;
        d.glowWidth = traits.bodyRadius * 0.25f;
        d.color = color;
        d.glowColor = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
        d.heading = a->GetHeading();
        m_InstanceData.push_back(d);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);
    glBufferData(GL_ARRAY_BUFFER, m_InstanceData.size() * sizeof(InstanceData), m_InstanceData.data(), GL_DYNAMIC_DRAW);
}

void AgentView::Draw(const glm::mat4& vp) {
    if (m_InstanceData.empty()) return;

    // Body
    m_ShaderBody.Bind();
    m_ShaderBody.SetMat4(Uniforms::MVP, vp);
    glBindVertexArray(m_VaoBody);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(m_InstanceData.size()));

    // Heading
    m_ShaderLine.Bind();
    m_ShaderLine.SetMat4(Uniforms::MVP, vp);
    glBindVertexArray(m_VaoLine);
    glDrawArraysInstanced(GL_LINES, 0, 2, static_cast<GLsizei>(m_InstanceData.size()));

    glBindVertexArray(0);
}

}  // namespace gr
