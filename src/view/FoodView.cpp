#include <glad.h>

#include <gramma/view/FoodView.hpp>

namespace gr {

void FoodView::Init() {
    m_Shader.BuildAgent();  // angenommen: du hast sowas wie einen CircleShader
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

void FoodView::Draw(const FoodSource* food, const glm::mat4& vp) {
    m_Shader.Bind();
    m_Shader.SetMat4("uMVP", vp);
    m_Shader.SetVec2("uPos", food->GetPosition());

    float baseRadius = 0.5f;                                    // fixed scale
    float radius = baseRadius * (0.5f + food->GetNutrition());  // skaliert mit Nutrition
    m_Shader.SetFloat("uRadius", radius);

    m_Shader.SetVec4("uColor", {0.2f, 0.8f, 0.2f, 1.0f});

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

}  // namespace gr
