#include <glm/gtc/matrix_transform.hpp>
#include <gramma/core/AppContext.hpp>
#include <gramma/view/Hud.hpp>

namespace gr {

Hud::Hud() = default;

Hud::~Hud() = default;

void Hud::Init() {
    m_QuadShapes.Init();
    m_Shader.BuildUnlit();
}

void Hud::AddPanel(std::unique_ptr<Panel> panel) {
    m_Panels.push_back(std::move(panel));
}

void Hud::Render(const AppContext& ctx) {
    m_QuadShapes.Clear();
    for (const auto& panel : m_Panels) {
        panel->Render();
    }
    m_QuadShapes.Upload();

    glm::mat4 proj = glm::ortho(0.0f, (float)ctx.GetWidth(), (float)ctx.GetHeight(), 0.0f);

    m_QuadShapes.Draw(m_Shader, proj, 1.0f);
}

void Hud::ClearPanels() {
    m_Panels.clear();
}

}  // namespace gr
