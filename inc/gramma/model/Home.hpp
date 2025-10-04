#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Agent.hpp>

namespace gr {

/** Represents a place where agents can rest */
class Home {
   public:
    Home(const glm::vec2& pos, int maxOccupancy = 4);

    const glm::vec2& GetPosition() const;

    bool CanEnter() const;
    bool Enter(Agent* agent);
    void Leave(Agent* agent);

    int GetMaxOccupancy() const {
        return m_MaxOccupancy;
    }

   private:
    glm::vec2 m_Position;
    std::vector<Agent*> m_Agents;
    int m_MaxOccupancy;
};

}  // namespace gr
