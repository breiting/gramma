#include <box2d/box2d.h>

#include <glm/glm.hpp>
#include <gramma/core/Math.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/resource/IResource.hpp>
#include <gramma/model/task/ITask.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include "gramma/core/Time.hpp"
#include "gramma/model/resource/IConsumable.hpp"

namespace gr {

Environment::Environment(const glm::vec2& gravity) {
    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = {gravity.x, gravity.y};
    m_World = b2CreateWorld(&def);
    // cellSize = max(AgentRadius * 2, SocialRadius * 0.75)
    m_SpatialGrid = std::make_unique<SpatialGrid>(0.25f);
}

Environment::~Environment() {
    b2DestroyWorld(m_World);
}

void Environment::AddBoundary(const std::vector<glm::vec2>& contour) {
    m_Boundary = contour;
    CreateChainShape(contour);
}

void Environment::AddObstacle(const std::vector<glm::vec2>& contour) {
    m_Obstacles.push_back(contour);
    CreateChainShape(contour);
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

void Environment::CreateChainShape(const std::vector<glm::vec2>& contour) {
    if (contour.size() < 4) {
        std::cerr << "Chain needs >= 4 points\n";
        return;
    }

    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(m_World, &bd);

    std::vector<b2Vec2> pts;
    pts.reserve(contour.size());
    for (const auto& p : contour) {
        pts.push_back({p.x, p.y});
    }

    b2ChainDef cd = b2DefaultChainDef();
    cd.points = pts.data();
    cd.count = static_cast<int>(pts.size());
    cd.isLoop = true;

    b2ChainId chain = b2CreateChain(body, &cd);

    b2Chain_SetFriction(chain, 0.1f);
    b2Chain_SetRestitution(chain, 0.0f);  // no repulsion (Abprallen)
}

void Environment::AddAgent(std::unique_ptr<Agent> agent) {
    glm::vec2 pos = agent->GetPosition();
    float mass = agent->GetTraits().mass;
    float radius = agent->GetTraits().bodyRadius;

    // Body definition
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_dynamicBody;
    bd.position = {pos.x, pos.y};

    // Create body in the world
    b2BodyId body = b2CreateBody(m_World, &bd);
    b2Body_SetFixedRotation(body, true);

    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = agent->GetTraits().bodyRadius;

    b2ShapeDef sd = b2DefaultShapeDef();
    sd.density = mass / (radius * radius * gr::PI);  // kg/m^2

    b2SurfaceMaterial mat = b2DefaultSurfaceMaterial();
    mat.friction = 0.3;
    mat.restitution = 0.2;
    sd.material = mat;

    b2CreateCircleShape(body, &sd, &circle);

    // register user data
    b2Body_SetUserData(body, agent.get());

    agent->SetBody(body);

    m_Agents.emplace_back(std::move(agent));
}

const Agent* Environment::GetAgent(size_t idx) const {
    return m_Agents[idx].get();
}

const std::vector<std::unique_ptr<Agent>>& Environment::GetAgents() const {
    return m_Agents;
}

void Environment::RemoveAllAgents() {
    for (auto& a : m_Agents) {
        b2DestroyBody(a->GetBody());
    }
    m_Agents.clear();
}

void Environment::AddResource(std::shared_ptr<IResource> r) {
    glm::vec2 pos = r->GetPosition();
    float radius = r->GetBoundingRadius();

    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(m_World, &bd);

    b2Circle circle;
    circle.center = {pos.x, pos.y};
    circle.radius = radius;

    b2ShapeDef sd = b2DefaultShapeDef();

    b2CreateCircleShape(body, &sd, &circle);

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
        TimeMeasureGuard guard("=== Box2D");

        b2World_Step(m_World, dt, 4);
    }

    {
        TimeMeasureGuard guard("Resource");

        // Update resources if required
        for (auto& r : m_Resources) {
            // All consumables
            if (auto* consumable = dynamic_cast<IConsumable*>(r.get())) {
                consumable->Regenerate(dt);
            }
        }
    }

    {
        TimeMeasureGuard guard("Agent Update");

        // Sync and update agents
        for (auto& agent : m_Agents) {
            if (agent->GetBody().index1 != 0) {
                b2Transform xf = b2Body_GetTransform(agent->GetBody());
                glm::vec2 pos = {xf.p.x, xf.p.y};
                agent->SetPosition(pos);
            }
            agent->Update(*this, dt);
        }
    }

    {
        TimeMeasureGuard guard("Grid");
        m_SpatialGrid->Clear();
        for (size_t i = 0; i < m_Agents.size(); ++i) {
            const glm::vec2& pos = m_Agents[i]->GetPosition();
            m_SpatialGrid->Insert(i, pos);
        }
    }

    {
        TimeMeasureGuard guard("Cleanup");

        // Delete dead agents
        auto it = m_Agents.begin();
        while (it != m_Agents.end()) {
            Agent* agent = it->get();
            if (agent->GetState() == AgentState::Dead || agent->GetState() == AgentState::Rescued) {
                b2DestroyBody(agent->GetBody());
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
                    b2DestroyBody(res->get()->GetBody());
                    res = m_Resources.erase(res);
                }
            } else {
                ++res;
            }
        }
    }
}

std::vector<const Agent*> Environment::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    std::vector<const Agent*> result;

    auto ids = m_SpatialGrid->QueryNeighborhood(pos, radius);
    for (size_t id : ids) {
        if (id >= m_Agents.size()) continue;
        const Agent* other = m_Agents[id].get();
        float dist = glm::length(other->GetPosition() - pos);
        if (dist <= radius) {
            result.push_back(other);
        }
    }

    return result;
}

#if 0
std::vector<const Agent*> Environment::QueryAgentsInRadius(const glm::vec2& center, float radius) const {
    std::vector<const Agent*> results;

    b2AABB aabb;
    aabb.lowerBound = {center.x - radius, center.y - radius};
    aabb.upperBound = {center.x + radius, center.y + radius};

    auto callback = [](b2ShapeId shapeId, void* context) -> bool {
        auto* data =
            static_cast<std::tuple<const Environment*, glm::vec2, float, std::vector<const Agent*>*>*>(context);
        const Environment* env = std::get<0>(*data);
        glm::vec2 center = std::get<1>(*data);
        float radius = std::get<2>(*data);
        auto* out = std::get<3>(*data);

        b2BodyId bodyId = b2Shape_GetBody(shapeId);
        void* userData = b2Body_GetUserData(bodyId);

        if (userData) {
            Agent* agent = static_cast<Agent*>(userData);
            float dist = glm::distance(agent->GetPosition(), center);
            if (dist <= radius) {
                out->push_back(agent);
            }
        }

        return true;
    };

    // Filter (match all)
    b2QueryFilter filter = b2DefaultQueryFilter();

    // Prepare context
    auto context = std::make_tuple(this, center, radius, &results);

    b2World_OverlapAABB(m_World, aabb, filter, callback, &context);

    return results;
}
#endif

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
