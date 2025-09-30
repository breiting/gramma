#pragma once

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/model/World2D.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/CircleShapes.hpp>
#include <gramma/view/Font.hpp>
#include <gramma/view/Image.hpp>
#include <gramma/view/LineShape.hpp>
#include <gramma/view/Panel.hpp>
#include <gramma/view/Shader.hpp>
#include <gramma/view/TextElement.hpp>
#include <memory>

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
    Shader m_CircleShader;
    Shader m_LineShader;
    Camera2D m_Camera;

    World2D m_World;
    CircleShapes m_Circles;
    LineShape m_Lines;
    bool m_Quit{false};

    std::unique_ptr<Panel> m_PerfPanel;
    std::unique_ptr<TextElement> m_fpsText;
    std::unique_ptr<Image> m_Image;
    Font m_font;
};
