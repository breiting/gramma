#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/ITask.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

namespace gr {

Environment::Environment(float xmin, float xmax, float ymin, float ymax)
    : m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax) {
}
glm::vec2 Environment::RandomPosition() const {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> xDist(XMin(), XMax());
    std::uniform_real_distribution<float> yDist(YMin(), YMax());
    return glm::vec2(xDist(rng), yDist(rng));
}

void Environment::AddAgent(std::unique_ptr<Agent> a) {
    m_Agents.emplace_back(std::move(a));
}
void Environment::AddResource(std::shared_ptr<IResource> r) {
    m_Resources.emplace_back(std::move(r));
}
void Environment::AddHome(std::shared_ptr<Home> h) {
    m_Homes.emplace_back(std::move(h));
}

void Environment::BuildSpatialIndex() {
    m_Cloud.agents = &m_Agents;
    if (!m_KDTree) {
        m_KDTree = std::make_unique<KDTreeType>(2, m_Cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    }
    m_KDTree->buildIndex();
}

void Environment::Update(float dt) {
    // Ressourcen regenerieren
    for (auto& r : m_Resources) r->Regenerate(dt);

    // KD-Tree for agents
    BuildSpatialIndex();

    // Agents updaten
    for (auto& a : m_Agents) {
        a->Update(dt, *this);
    }

    // Physics
    if (m_CollisionHandler) {
        m_CollisionHandler->Resolve(*this);
    }

    // Delete dead agents
    m_Agents.erase(std::remove_if(m_Agents.begin(), m_Agents.end(),
                                  [](const std::unique_ptr<Agent>& a) {
                                      return a->GetState() == AgentState::Dead;  //
                                  }),
                   m_Agents.end());

    // Delete empty resources
    m_Resources.erase(std::remove_if(m_Resources.begin(), m_Resources.end(),
                                     [](std::shared_ptr<IResource>& r) {
                                         return r->IsDepleted();  //
                                     }),
                      m_Resources.end());
}

std::shared_ptr<IResource> Environment::FindNearest(ResourceType type, const glm::vec2& pos) const {
    float bestD2 = std::numeric_limits<float>::infinity();
    std::shared_ptr<IResource> best = nullptr;
    for (auto& r : m_Resources) {
        if (r->GetType() != type || r->IsDepleted()) continue;
        glm::vec2 d = r->GetPosition() - pos;
        float d2 = glm::dot(d, d);
        if (d2 < bestD2) {
            bestD2 = d2;
            best = r;
        }
    }
    return best;
}

void Environment::Stats() const {
    size_t numAgents = m_Agents.size();
    size_t numResources = m_Resources.size();

    // Ausgabe formatiert
    std::cout << "=== Environment Stats ===\n"
              << "Agents: " << numAgents << "  |  Resources: " << numResources << "\n"
              << "-------------------------\n";
}

}  // namespace gr
