#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace gr {

/** Instanced circles in world units (meters). */
class CircleBatch {
   public:
    struct Instance {
        glm::vec2 pos;
        float diameter;
    };  // diameter = 2*radius
    bool init();
    void clear();  // remove all instances
    void add(const glm::vec2& pos, float diameter);
    void upload();  // send to GPU
    void draw(const glm::mat4& vp, float alpha = 1.0f) const;

   private:
    unsigned m_vao = 0, m_vboQuad = 0, m_vboInst = 0, m_prog = 0;
    std::vector<Instance> m_data;
};

}  // namespace gr
