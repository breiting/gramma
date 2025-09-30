#include "HelloApp.hpp"

#include <glad.h>

#include <gramma/core/IApp.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/Shader.hpp>
#include <iostream>

using namespace gr;

std::string HelloApp::Name() const {
    return "HelloApp";
}

bool HelloApp::Init(gr::AppContext& ctx) {
    // Tiny triangle to verify pipeline
    std::vector<float> tri = {-0.5f, -0.3f, 0.5f, -0.3f, 0.0f, 0.4f};
    m_mesh.Create(tri, 2);

    m_cam.SetOrtho(-1.6f, 1.6f, -0.9f, 0.9f);
    try {
        m_shader.BuildUnlit();
    } catch (const std::exception& e) {
        std::cerr << "Shader build failed: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void HelloApp::Update(gr::AppContext& ctx, double /*dt*/) {
    // nothing yet
}

void HelloApp::Render(gr::AppContext& ctx) {
    m_shader.Bind();
    m_shader.SetMat4("uMVP", m_cam.ViewProj());
    m_shader.SetVec3("uColor", {1.0f, 1.0f, 1.0f});
    m_mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_mesh.VertexCount());
}
