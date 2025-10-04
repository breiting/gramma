#include <algorithm>
#include <cassert>
#include <gramma/model/Home.hpp>

namespace gr {

Home::Home(const glm::vec2& pos, int maxOccupancy) : m_Position(pos), m_MaxOccupancy(maxOccupancy) {
    if (m_MaxOccupancy < 0) m_MaxOccupancy = 0;
    m_Agents.reserve(static_cast<size_t>(m_MaxOccupancy));
}

const glm::vec2& Home::GetPosition() const {
    return m_Position;
}

bool Home::CanEnter() const {
    return static_cast<int>(m_Agents.size()) < m_MaxOccupancy;
}

bool Home::Enter(Agent* agent) {
    if (!agent) return false;

    auto it = std::find(m_Agents.begin(), m_Agents.end(), agent);
    if (it != m_Agents.end()) return false;

    if (!CanEnter()) return false;

    m_Agents.push_back(agent);
    return true;
}

void Home::Leave(Agent* agent) {
    if (!agent) return;

    auto it = std::find(m_Agents.begin(), m_Agents.end(), agent);
    if (it != m_Agents.end()) {
        m_Agents.erase(it);
    }
}

}  // namespace gr
