#include <box2d/box2d.h>

#include <glm/glm.hpp>
#include <gramma/core/Math.hpp>
#include <gramma/core/Time.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/physics/Box2DPhysicsEngine.hpp>
#include <gramma/model/physics/SimpleGridPhysicsEngine.hpp>
#include <gramma/model/resource/IConsumable.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/ITask.hpp>
#include <limits>
#include <memory>
#include <random>

namespace gr {

Environment::Environment() {
    // m_Physics = std::make_unique<SimpleGridPhysicsEngine>(0.5f);
    m_Physics = std::make_unique<Box2DPhysicsEngine>();
    m_Physics->Init();
}

Environment::~Environment() {
    m_Physics.reset();
}

void Environment::AddBoundary(const std::vector<glm::vec2>& contour) {
    m_Boundary = contour;
    m_Physics->AddBoundary(contour);
}

void Environment::AddObstacle(const std::vector<glm::vec2>& contour) {
    m_Obstacles.push_back(contour);
    m_Physics->AddObstacle(contour);
}

IPhysicsEngine* Environment::GetPhysics() {
    return m_Physics.get();
}

bool Environment::ContainsPoint(const glm::vec2& p) const {
    bool inside = false;
    int n = (int)m_Boundary.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        const glm::vec2& pi = m_Boundary[i];
        const glm::vec2& pj = m_Boundary[j];

        bool intersect =
            ((pi.y > p.y) != (pj.y > p.y)) && (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y + 1e-6f) + pi.x);
        if (intersect) inside = !inside;
    }
    return inside;
}

glm::vec2 Environment::RandomPosition() const {
    if (m_Boundary.empty()) return {0, 0};

    float minX = m_Boundary[0].x, maxX = m_Boundary[0].x;
    float minY = m_Boundary[0].y, maxY = m_Boundary[0].y;
    for (auto& p : m_Boundary) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    // RNG
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    // Rejection-Sampling
    for (int tries = 0; tries < 1000; ++tries) {
        glm::vec2 candidate(distX(rng), distY(rng));
        if (ContainsPoint(candidate)) {
            return candidate;
        }
    }

    // Fallback: Mittelpunkt des Bounding Box
    return {(minX + maxX) * 0.5f, (minY + maxY) * 0.5f};
}

void Environment::AddAgent(std::unique_ptr<Agent> agent) {
    m_Physics->AddAgent(*agent);
    m_Agents.emplace_back(std::move(agent));
}

const Agent* Environment::GetAgent(size_t idx) const {
    if (idx < m_Agents.size()) {
        return m_Agents[idx].get();
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Agent>>& Environment::GetAgents() const {
    return m_Agents;
}

void Environment::RemoveAllAgents() {
    for (auto& a : m_Agents) {
        m_Physics->RemoveAgent(*a);
    }
    m_Agents.clear();
}

void Environment::AddResource(std::shared_ptr<IResource> r) {
    m_Physics->AddResource(r);
    m_Resources.emplace_back(std::move(r));
}

const std::vector<std::shared_ptr<IResource>>& Environment::GetResources() const {
    return m_Resources;
}

const std::vector<glm::vec2>& Environment::GetBoundary() const {
    return m_Boundary;
}

const std::vector<std::vector<glm::vec2>>& Environment::GetObstacles() const {
    return m_Obstacles;
}

void Environment::Update(float dt) {
    {
        TimeMeasureGuard guard("=== Physics");

        m_Physics->Step(dt);
    }

    {
        TimeMeasureGuard guard("Resource");

        for (auto& r : m_Resources) {
            if (auto* consumable = dynamic_cast<IConsumable*>(r.get())) {
                consumable->Regenerate(dt);
            }
        }
    }

    {
        TimeMeasureGuard guard("Agent Update");

        for (auto& agent : m_Agents) {
            m_Physics->SyncAgentState(*agent);
            agent->Update(*this, dt);
        }
    }

    {
        TimeMeasureGuard guard("Cleanup");

        auto it = m_Agents.begin();
        while (it != m_Agents.end()) {
            Agent* agent = it->get();
            if (agent->GetState() == AgentState::Dead || agent->GetState() == AgentState::Rescued) {
                m_Physics->RemoveAgent(*agent);
                it = m_Agents.erase(it);
            } else {
                ++it;
            }
        }

        // Delete finally consumed resources
        auto res = m_Resources.begin();
        while (res != m_Resources.end()) {
            if (auto* consumable = dynamic_cast<IConsumable*>(res->get())) {
                if (consumable->IsDepleted()) {
                    m_Physics->RemoveResource(res->get());
                    res = m_Resources.erase(res);
                }
            } else {
                ++res;
            }
        }
    }
}

std::vector<const Agent*> Environment::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    return m_Physics->QueryAgentsInRadius(pos, radius);
}

std::shared_ptr<IResource> Environment::FindNearest(ResourceType type, const glm::vec2& pos) const {
    float bestD2 = std::numeric_limits<float>::infinity();
    std::shared_ptr<IResource> best = nullptr;
    for (auto& r : m_Resources) {
        if (r->GetType() != type) continue;
        glm::vec2 d = r->GetPosition() - pos;
        float d2 = glm::dot(d, d);
        if (d2 < bestD2) {
            bestD2 = d2;
            best = r;
        }
    }
    return best;
}

}  // namespace gr
