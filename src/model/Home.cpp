#include <algorithm>
#include <gramma/model/Home.hpp>

namespace gr {

bool Home::Enter(Agent* agent) {
    if (!agent) return false;
    if (!CanEnter()) return false;
    if (std::find(m_Agents.begin(), m_Agents.end(), agent) != m_Agents.end()) return false;
    m_Agents.push_back(agent);
    return true;
}

void Home::Leave(Agent* agent) {
    if (!agent) return;
    auto it = std::find(m_Agents.begin(), m_Agents.end(), agent);
    if (it != m_Agents.end()) m_Agents.erase(it);
}

}  // namespace gr
