// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <gramma/model/SegregationTraits.hpp>
#include <gramma/view/AgentBatchView.hpp>
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

static glm::vec4 GroupColor(Group group) {
    switch (group) {
        case Group::Blue:
            return {0.0f, 0.0f, 1.0f, 1.0f};
        case Group::Red:
            return {1.0f, 0.0f, 0.0f, 1.0f};
    }
    return {1.0f, 1.0f, 1.0f, 1.0f};  // fallback
}

AgentBatchView::AgentBatchView() {
}
AgentBatchView::~AgentBatchView() {
    if (m_InstanceVbo) glDeleteBuffers(1, &m_InstanceVbo);
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
}

void AgentBatchView::Init() {
    m_Shader.BuildAgentBatch();

    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);

    glGenBuffers(1, &m_InstanceVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);

    // Layout (binding für Shader-Instanced Attributes)
    std::size_t stride = sizeof(InstanceData);

    // position (vec2), innerRadius, outerRadius, blendWidth
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, innerRadius));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, outerRadius));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, blendWidth));
    glVertexAttribDivisor(4, 1);

    // color
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(5, 1);

    // glowColor
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, glowColor));
    glVertexAttribDivisor(6, 1);

    // thickness
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, thickness));
    glVertexAttribDivisor(7, 1);

    // heading
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, heading));
    glVertexAttribDivisor(8, 1);

    // fov
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, fov));
    glVertexAttribDivisor(9, 1);

    glBindVertexArray(0);
}

void AgentBatchView::UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents) {
    m_InstanceData.clear();
    m_InstanceData.reserve(agents.size());

    for (auto& a : agents) {
        if (a->GetState() == AgentState::Dead) continue;
        const auto& traits = a->GetTraits();

        auto color = AgeColor(traits->age);
        const auto* segTrait = a->GetTraitsAs<SegregationTraits>();
        if (segTrait) {
            color = GroupColor(segTrait->group);
        }

        InstanceData d;
        d.position = a->GetPosition();
        d.innerRadius = traits->bodyRadius;
        d.outerRadius = traits->socialRadius;
        d.blendWidth = traits->bodyRadius * 0.1f;
        d.color = color;
        d.glowColor = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
        d.thickness = 0.1f;
        d.heading = glm::radians(a->GetHeading());
        d.fov = 30.0f;
        m_InstanceData.push_back(d);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVbo);
    glBufferData(GL_ARRAY_BUFFER, m_InstanceData.size() * sizeof(InstanceData), m_InstanceData.data(), GL_DYNAMIC_DRAW);
}

void AgentBatchView::Draw(const glm::mat4& vp) {
    if (m_InstanceData.empty()) return;

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);

    glBindVertexArray(m_Vao);

    // 4 vertices für ein Quad → instanziert für alle Agents
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(m_InstanceData.size()));

    glBindVertexArray(0);
}

}  // namespace gr
