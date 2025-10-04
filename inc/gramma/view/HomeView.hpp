#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

/**
 * Renders a Home as a hexagon outline.
 */
class HomeView {
   public:
    HomeView();
    ~HomeView();

    void Init();
    void Draw(const Home* home, const glm::mat4& vp);

   private:
    GLuint m_Vao = 0;
    GLuint m_Vbo = 0;
    Shader m_Shader;
    int m_VertexCount = 0;
};

}  // namespace gr
