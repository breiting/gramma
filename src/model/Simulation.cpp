#define GLM_ENABLE_EXPERIMENTAL
#include <array>
#include <glm/geometric.hpp>
#include <gramma/model/Simulation.hpp>
#include <limits>
#include <random>

namespace gr {

Simulation::Simulation(const Room& room, std::unique_ptr<INavigationStrategy> navStrategy,
                       std::unique_ptr<ICollisionHandler> collisionHandler, int numAgents)
    : m_Room(room),
      m_NavStrategy(std::move(navStrategy)),
      m_CollisionHandler(std::move(collisionHandler)),
      m_NumAgents(numAgents) {
}

void Simulation::Init() {
    m_Agents.clear();
    m_Agents.reserve(m_NumAgents);
    m_ElapsedTime = 0.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posX(m_Room.Pos.x, m_Room.Pos.x + m_Room.Size.x);
    std::uniform_real_distribution<float> posY(m_Room.Pos.y, m_Room.Pos.y + m_Room.Size.y);

    for (int i = 0; i < m_NumAgents; ++i) {
        Agent agent;
        agent.Pos = glm::vec2(posX(gen), posY(gen));
        agent.Vel = glm::vec2(0.0f);
        agent.traits = RandomTraits();
        agent.chosenExitIndex = -1;
        m_Agents.push_back(agent);
    }
}

void Simulation::Step(float dt) {
    m_ElapsedTime += dt;

    // Choose exits for agents
    for (auto& agent : m_Agents) {
        if (agent.chosenExitIndex == -1) {
            float minDist = std::numeric_limits<float>::max();
            int bestIndex = -1;
            for (size_t i = 0; i < m_Room.exits.size(); ++i) {
                float dist = glm::distance(agent.Pos, m_Room.exits[i].Pos);
                if (dist < minDist) {
                    minDist = dist;
                    bestIndex = static_cast<int>(i);
                }
            }
            agent.chosenExitIndex = bestIndex;
        }
    }

    // Compute desired velocities
    for (auto& agent : m_Agents) {
        const Exit& exit = m_Room.exits[agent.chosenExitIndex];
        glm::vec2 desiredVel = m_NavStrategy->ComputeDesiredVelocity(agent, exit);
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
                                      const Exit& exit = m_Room.exits[agent.chosenExitIndex];
                                      return agent.Pos.x >= exit.Pos.x - exit.Size.x / 2 &&
                                             agent.Pos.x <= exit.Pos.x + exit.Size.x / 2 &&
                                             agent.Pos.y >= exit.Pos.y - exit.Size.y / 2 &&
                                             agent.Pos.y <= exit.Pos.y + exit.Size.y / 2;
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

float Simulation::GetElapsedTime() const {
    return m_ElapsedTime;
}

std::array<int, 4> Simulation::GetAgeCounts() const {
    std::array<int, 4> counts = {0, 0, 0, 0};
    for (const auto& agent : m_Agents) {
        counts[static_cast<int>(agent.traits.age)]++;
    }
    return counts;
}

}  // namespace gr