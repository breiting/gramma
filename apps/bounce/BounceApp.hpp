#pragma once

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/model/World2D.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/CircleBatch.hpp>
#include <gramma/view/LineBatch.hpp>
#include <gramma/view/Shader.hpp>

using namespace gr;

class BounceApp : public gr::IApp {
   public:
    BounceApp() = default;
    ~BounceApp() = default;
    std::string Name() const override;

    bool Init(AppContext& ctx) override;
    void Update(AppContext& ctx, double /*dt*/) override;

    void Render(AppContext& ctx) override;
    void Ui(gr::AppContext& ctx) override;

    void HandleKeyPressedEvent(int key, int mods) override;
    void HandleMouseButtonEvent(int button, int action, int mods) override;
    void HandleMouseMoveEvent(double x, double y) override;

   private:
    GLuint m_vao = 0, m_vbo = 0;
    Shader m_shader;
    Camera2D m_cam;

    World2D m_world;
    CircleBatch m_circles;
    LineBatch m_lines;
    bool m_Quit{false};
};
