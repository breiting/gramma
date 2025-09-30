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
    std::vector<float> tri = {-0.5f, -0.3f, 1, 1, 1, 1, 0.5f, -0.3f, 1, 1, 1, 1, 0.0f, 0.4f, 1, 1, 1, 1};
    m_Mesh.Create(tri, 6);

    m_Cam.SetOrtho(-1.6f, 1.6f, -0.9f, 0.9f);
    try {
        m_Shader.BuildUnlit();
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
    m_Mesh.Draw(m_Shader, m_Cam.ViewProj(), 1.0f);
}
