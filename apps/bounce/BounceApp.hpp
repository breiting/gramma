#pragma once

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/model/World2D.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/CircleBatch.hpp>
#include <gramma/view/Font.hpp>
#include <gramma/view/LineBatch.hpp>
#include <gramma/view/Panel.hpp>
#include <memory>

#include "../../inc/gramma/view/Shader.hpp"

using namespace gr;

class BounceApp : public gr::IApp {
   public:
    BounceApp() = default;
    ~BounceApp() = default;
    std::string Name() const override;

    bool Init(AppContext& ctx) override;
    void Update(AppContext& ctx, double /*dt*/) override;

    void Render(AppContext& ctx) override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    Shader m_circleShader;
    Shader m_lineShader;
    Camera2D m_cam;

    World2D m_world;
    CircleBatch m_circles;
    LineBatch m_lines;
    bool m_Quit{false};

    std::unique_ptr<Panel> m_PerfPanel;
    // TODO: Add text elements when font is fixed
    // TextElement* m_fpsText;
    // TextElement* m_upsText;
    // Font m_font;
};
