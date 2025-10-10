#pragma once
#include <algorithm>
#include <gramma/model/resource/IResource.hpp>

namespace gr {

class FoodResource : public IResource {
   public:
    FoodResource(const glm::vec2& pos, float capacity = 1.0f, float regenRate = 0.01f, float maxCap = 1.0f)
        : m_Pos(pos), m_Capacity(capacity), m_RegenRate(regenRate), m_Max(maxCap) {
    }

    ResourceType GetType() const override {
        return ResourceType::Food;
    }
    const glm::vec2& GetPosition() const override {
        return m_Pos;
    }

    float Consume(float amount) override {
        float take = std::min(amount, m_Capacity);
        m_Capacity -= take;
        return take;
    }

    void Regenerate(float dt) override {
        m_Capacity = std::min(m_Max, m_Capacity + m_RegenRate * dt);
    }

    bool IsDepleted() const override {
        return m_Capacity <= 0.0f;
    }

    float Capacity() const {
        return m_Capacity;
    }
    float MaxCapacity() const {
        return m_Max;
    }

   private:
    glm::vec2 m_Pos;
    float m_Capacity;
    float m_RegenRate;
    float m_Max;
};

}  // namespace gr
