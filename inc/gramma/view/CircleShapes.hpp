#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/view/IRenderable.hpp>
#include <vector>

namespace gr {

class Shader;

/** Instanced circles in world units (meters). */
class CircleShapes : public IRenderable {
   public:
    struct Instance {
        glm::vec2 Pos;
        float Diameter;
        glm::vec4 Color;
    };  // diameter = 2*radius
    void Init() override;
    void Clear() override;  // remove all instances
    void Add(const glm::vec2& pos, float diameter, const glm::vec4& color = glm::vec4(1.0f));
    void Upload() override;  // send to GPU
    void Draw(const Shader& shader, const glm::mat4& vp, float alpha = 1.0f) override;

   private:
    unsigned m_Vao = 0, m_VboQuad = 0, m_VboInst = 0;
    std::vector<Instance> m_Data;
};

}  // namespace gr
