#pragma once
#include <memory>
#include <vector>

#include "Panel.hpp"
#include "QuadShapes.hpp"
#include "Shader.hpp"
#include "TexturedQuadShapes.hpp"

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
    TexturedQuadShapes& GetQuadBatch() {
        return m_TexturedQuadShapes;
    }

    /** Get the textured quad shapes for elements. */
    TexturedQuadShapes& GetTexturedQuadBatch() {
        return m_TexturedQuadShapes;
    }

   private:
    TexturedQuadShapes m_TexturedQuadShapes;
    Shader m_ColoredShader;
    Shader m_TexturedShader;
    std::vector<std::unique_ptr<Panel>> m_Panels;
};

}  // namespace gr