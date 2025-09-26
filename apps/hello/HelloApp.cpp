#include "HelloApp.hpp"

#include <glad.h>

#include <gramma/Camera2D.hpp>
#include <gramma/IApp.hpp>
#include <gramma/Shader.hpp>

using namespace gramma;

HelloApp::~HelloApp() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

const char* HelloApp::Name() const {
    return "HelloApp";
}

bool HelloApp::Init() {
    // Tiny triangle to verify pipeline
    float tri[6] = {-0.5f, -0.3f, 0.5f, -0.3f, 0.0f, 0.4f};
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_cam.SetOrtho(-1.6f, 1.6f, -0.9f, 0.9f);
    m_shader.BuildUnlit();
    return true;
}

void HelloApp::Update(double /*dt*/) {
    // nothing yet
}

void HelloApp::Render() {
    m_shader.Bind();
    m_shader.SetMat4("uMVP", m_cam.ViewProj());
    m_shader.SetVec3("uColor", {1.0f, 1.0f, 1.0f});
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
