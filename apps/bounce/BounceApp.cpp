#include "BounceApp.hpp"

#include <imgui.h>

#include <cstdlib>
#include <gramma/core/Runner.hpp>

#include "GLFW/glfw3.h"

using namespace gr;

std::string BounceApp::Name() const {
    return "BallBounce";
}

bool BounceApp::Init(gr::AppContext &ctx) {
    // View
    m_cam.SetOrthoByHeight(10, ctx.Aspect());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_circles.init();
    m_lines.init();

    // Model
    m_world.gravity = {0.f, -9.81f};
    m_world.groundY = -0.6f;
    m_world.groundFriction = 0.15f;
    // one dynamic ball (mass=1kg, r=0.1m, e=0.65)
    //
    for (int i = -5; i < 6; i++) {
        auto idx = m_world.addBody(Body2D::Dynamic(0.1f, 0.10f, abs(i * 0.1) + 0.3));
        m_world.body(idx).pos = {i, 5.f};
        m_world.body(idx).vel = {0.0f, 0.0f};
    }

    // ground line for rendering
    m_lines.set({{-10.f, m_world.groundY}, {10.f, m_world.groundY}});
    return true;
}

void BounceApp::HandleKeyPressedEvent(int key, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        m_Quit = true;
    }
}
void BounceApp::HandleMouseButtonEvent(int button, int action, int mods) {
    printf("%d\n", button);
}
void BounceApp::HandleMouseMoveEvent(double x, double y) {
    printf("%3.2f %3.2f\n", x, y);
}

void BounceApp::Update(gr::AppContext &ctx, double dt) {
    // fixed step is fine here (or use dt)
    m_world.Step((float)dt);
}

void BounceApp::Render(gr::AppContext &ctx) {
    if (m_Quit) {
        ctx.RequestQuit();
        return;
    }

    glm::mat4 vp = m_cam.ViewProj();

    // circles
    m_circles.clear();
    for (auto &b : m_world.bodies()) {
        m_circles.add(b.pos, 2.0f * b.radius);
    }
    m_circles.upload();
    m_lines.draw(vp, 0.8f);
    m_circles.draw(vp, 0.95f);
}

void BounceApp::Ui(gr::AppContext &ctx) {
    ImGui::Begin("Performance");
    ImGui::Text("Update dt: %.3f ms (%.1f UPS)", ctx.GetUpdateDt() * 1000.0, 1.0 / ctx.GetUpdateDt());
    ImGui::Text("Frame dt:  %.3f ms (%.1f FPS)", ctx.GetFrameDt() * 1000.0, 1.0 / ctx.GetFrameDt());
    ImGui::Button("Reset");
    ImGui::End();
}
