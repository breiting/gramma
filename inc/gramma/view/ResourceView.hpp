#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/view/Shader.hpp>

namespace gr {

/** Simple visual representation of a resource */
class ResourceView {
   public:
    void Init();
    void Draw(const IResource* res, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_VAO = 0, m_VBO = 0;
};

}  // namespace gr
