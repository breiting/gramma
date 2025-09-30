#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <gramma/model/Simulation.hpp>
#include <random>

namespace gr {

Simulation::Simulation(const Room& room, const Exit& exit, std::unique_ptr<INavigationStrategy> navStrategy,
                       std::unique_ptr<ICollisionHandler> collisionHandler, int numAgents)
    : m_Room(room),
      m_Exit(exit),
      m_NavStrategy(std::move(navStrategy)),
      m_CollisionHandler(std::move(collisionHandler)),
      m_NumAgents(numAgents) {
}

void Simulation::Init() {
    m_Agents.clear();
    m_Agents.reserve(m_NumAgents);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posX(m_Room.Pos.x, m_Room.Pos.x + m_Room.Size.x);
    std::uniform_real_distribution<float> posY(m_Room.Pos.y, m_Room.Pos.y + m_Room.Size.y);
    std::uniform_real_distribution<float> speed(0.5f, 1.5f);   // m/s
    std::uniform_real_distribution<float> radius(0.2f, 0.5f);  // meters

    for (int i = 0; i < m_NumAgents; ++i) {
        Agent agent;
        agent.Pos = glm::vec2(posX(gen), posY(gen));
        agent.Vel = glm::vec2(0.0f);
        agent.Radius = radius(gen);
        agent.SpeedPref = speed(gen);
        m_Agents.push_back(agent);
    }
}

void Simulation::Step(float dt) {
    // Compute desired velocities
    for (auto& agent : m_Agents) {
        glm::vec2 desiredVel = m_NavStrategy->ComputeDesiredVelocity(agent, m_Exit);
        agent.Vel = desiredVel;  // for simplicity, set directly
    }

    // Handle collisions
    m_CollisionHandler->HandleCollisions(m_Agents);

    // Update positions
    for (auto& agent : m_Agents) {
        agent.Pos += agent.Vel * dt;
    }

    // Check for exit
    m_Agents.erase(std::remove_if(m_Agents.begin(), m_Agents.end(),
                                  [this](const Agent& agent) {
                                      return agent.Pos.x >= m_Exit.Pos.x - m_Exit.Size.x / 2 &&
                                             agent.Pos.x <= m_Exit.Pos.x + m_Exit.Size.x / 2 &&
                                             agent.Pos.y >= m_Exit.Pos.y - m_Exit.Size.y / 2 &&
                                             agent.Pos.y <= m_Exit.Pos.y + m_Exit.Size.y / 2;
                                  }),
                   m_Agents.end());
}

bool Simulation::IsComplete() const {
    return m_Agents.empty();
}

const std::vector<Agent>& Simulation::GetAgents() const {
    return m_Agents;
}

const Room& Simulation::GetRoom() const {
    return m_Room;
}

const Exit& Simulation::GetExit() const {
    return m_Exit;
}

}  // namespace gr