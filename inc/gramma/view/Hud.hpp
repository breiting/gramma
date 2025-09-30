#pragma once
#include <memory>
#include <vector>

#include "Panel.hpp"
#include "QuadBatch.hpp"
#include "Shader.hpp"

namespace gr {

class AppContext;

/** Main HUD manager. */
class Hud {
   public:
    Hud();
    ~Hud();

    /** Initialize the HUD. */
    void Init();

    /** Add a panel to the HUD. */
    void AddPanel(std::unique_ptr<Panel> panel);

    /** Render all panels. */
    void Render(const AppContext& ctx);

    /** Clear all panels (for dynamic updates). */
    void ClearPanels();

    /** Get the quad batch for creating panels. */
    QuadBatch& GetQuadBatch() {
        return m_quadBatch;
    }

   private:
    QuadBatch m_quadBatch;
    Shader m_shader;
    std::vector<std::unique_ptr<Panel>> m_panels;
};

}  // namespace gr