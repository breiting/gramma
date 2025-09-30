#include <gramma/core/AppContext.hpp>
#include <gramma/view/Hud.hpp>

namespace gr {

Hud::Hud() = default;

Hud::~Hud() = default;

void Hud::Init() {
    m_quadBatch.Init();
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
    // Bind a simple shader for quads (assume unlit)
    // For now, assume Shader is set up
    m_quadBatch.Draw();
}

void Hud::ClearPanels() {
    m_panels.clear();
}

}  // namespace gr