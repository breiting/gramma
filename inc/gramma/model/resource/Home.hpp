#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "gramma/model/resource/IResource.hpp"

namespace gr {
class Agent;

class Home : public IResource {
   public:
    Home(const glm::vec2& pos, int maxOccupancy = 4) : m_Position(pos), m_MaxOccupancy(maxOccupancy) {
    }

    ResourceType GetType() const override {
        return ResourceType::Home;
    }

    const glm::vec2& GetPosition() const override {
        return m_Position;
    }

    float GetBoundingRadius() const override {
        return 0.5;
    }

    bool CanEnter() const {
        return static_cast<int>(m_Agents.size()) < m_MaxOccupancy;
    }
    bool Enter(Agent* agent);
    void Leave(Agent* agent);

    int Capacity() const {
        return m_MaxOccupancy;
    }
    int Occupancy() const {
        return static_cast<int>(m_Agents.size());
    }
    int GetMaxOccupancy() const {
        return m_MaxOccupancy;
    }

    // Pantry
    void Deposit(float e) {
        m_Pantry += e;
        if (m_Pantry < 0) m_Pantry = 0;
    }
    float Withdraw(float e) {
        float t = (e < m_Pantry) ? e : m_Pantry;
        m_Pantry -= t;
        return t;
    }
    float Pantry() const {
        return m_Pantry;
    }

    const std::vector<Agent*>& Residents() const {
        return m_Agents;
    }

   private:
    glm::vec2 m_Position;
    std::vector<Agent*> m_Agents;
    int m_MaxOccupancy;
    float m_Pantry{0.0f};
};

}  // namespace gr
