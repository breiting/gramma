#pragma once
#include <glm/vec2.hpp>

namespace gr {

/** Represents a place where agents can eat to satisfy hunger */
class FoodSource {
   public:
    FoodSource(const glm::vec2& pos, float nutrition = 1.0f);

    const glm::vec2& GetPosition() const;
    float Consume();

   private:
    glm::vec2 m_Position;
    float m_Nutrition;  // available food (0..1 for simplicity)
};

}  // namespace gr
