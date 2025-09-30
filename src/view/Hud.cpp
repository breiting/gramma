#include <glm/gtc/matrix_transform.hpp>
#include <gramma/core/AppContext.hpp>
#include <gramma/view/Hud.hpp>

namespace gr {

Hud::Hud() = default;

Hud::~Hud() = default;

void Hud::Init() {
    m_TexturedQuadShapes.Init();
    m_ColoredShader.BuildUnlit();
    m_TexturedShader.BuildUnlit();
}

void Hud::AddPanel(std::unique_ptr<Panel> panel) {
    m_Panels.push_back(std::move(panel));
}

void Hud::Render(const AppContext& ctx) {
    m_TexturedQuadShapes.Clear();
    for (const auto& panel : m_Panels) {
        panel->Render();
    }
    m_TexturedQuadShapes.Upload();

    glm::mat4 proj = glm::ortho(0.0f, (float)ctx.GetWidth(), (float)ctx.GetHeight(), 0.0f);

    m_TexturedQuadShapes.Draw(m_ColoredShader, m_TexturedShader, proj, glm::vec4{1.0f});
}

void Hud::ClearPanels() {
    m_Panels.clear();
}

}  // namespace gr
