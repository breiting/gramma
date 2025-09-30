#pragma once
#include <memory>
#include <vector>

#include "Panel.hpp"
#include "QuadShapes.hpp"
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

    /** Get the quad shapes for creating panels. */
    QuadShapes& GetQuadBatch() {
        return m_QuadShapes;
    }

   private:
    QuadShapes m_QuadShapes;
    Shader m_Shader;
    std::vector<std::unique_ptr<Panel>> m_Panels;
};

}  // namespace gr