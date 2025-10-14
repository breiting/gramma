#pragma once
#include <glm/glm.hpp>
#include <string>

namespace gr {

class Particle {
   public:
    explicit Particle(std::string id, const glm::vec2& position, int group, float radius = 0.1);

    const std::string& GetId() const;
    const glm::vec2& GetPosition() const;
    void SetPosition(const glm::vec2& pos);

    const glm::vec2& GetVelocity() const;
    void SetVelocity(const glm::vec2& vel);

    float GetRadius() const;

    int GetGroup() const;

   private:
    std::string m_Id;
    glm::vec2 m_Position;
    glm::vec2 m_Velocity{0.0f};
    int m_Group;
    float m_Radius;
};

}  // namespace gr
