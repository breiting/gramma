#pragma once
#include <glm/vec2.hpp>

namespace gr {

/** Represents a place where agents can eat to satisfy hunger */
class FoodSource {
   public:
    FoodSource(const glm::vec2& pos, float nutrition = 1.0f, float regenRate = 0.02f);

    const glm::vec2& GetPosition() const;

    float Consume(float amount);  // agent eats, returns actual amount consumed
    void Regenerate(float dt);    // regenerates nutrition over time

    float GetNutrition() const;

   private:
    glm::vec2 m_Position;
    float m_Nutrition;  // [0..1]
    float m_RegenRate;  // per second
};

}  // namespace gr
