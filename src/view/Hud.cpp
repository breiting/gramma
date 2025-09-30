#include <glm/gtc/matrix_transform.hpp>
#include <gramma/core/AppContext.hpp>
#include <gramma/view/Hud.hpp>

namespace gr {

Hud::Hud() = default;

Hud::~Hud() = default;

void Hud::Init() {
    m_quadBatch.Init();
    m_shader.BuildUnlit();
}

void Hud::AddPanel(std::unique_ptr<Panel> panel) {
    m_panels.push_back(std::move(panel));
}

void Hud::Render(const AppContext& ctx) {
    m_quadBatch.Clear();
    for (const auto& panel : m_panels) {
        panel->Render();
    }
    m_quadBatch.Upload();

    glm::mat4 proj = glm::ortho(0.0f, (float)ctx.GetWidth(), (float)ctx.GetHeight(), 0.0f);
    m_shader.Bind();
    m_shader.SetMat4("uMVP", proj);

    m_quadBatch.Draw(m_shader);
}

void Hud::ClearPanels() {
    m_panels.clear();
}

}  // namespace gr
