#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/ITask.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include "box2d/math_functions.h"

namespace gr {

Environment::Environment(const glm::vec2& gravity) {
    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = {gravity.x, gravity.y};
    m_World = b2CreateWorld(&def);
}

Environment::~Environment() {
    b2DestroyWorld(m_World);
}

glm::vec2 Environment::RandomPosition() const {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> xDist(-10, 10);
    std::uniform_real_distribution<float> yDist(-10, 10);
    return glm::vec2(xDist(rng), yDist(rng));
}

void Environment::AddBoundary(const std::vector<glm::vec2>& vertices) {
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(m_World, &bd);

    for (size_t i = 0; i < vertices.size(); ++i) {
        auto a = vertices[i];
        auto b = vertices[(i + 1) % vertices.size()];

        b2Segment segment;
        segment.point1 = {a.x, a.y};
        segment.point2 = {b.x, b.y};

        b2ShapeDef sd = b2DefaultShapeDef();
        b2CreateSegmentShape(body, &sd, &segment);
    }
    m_Boundary = vertices;
}

void Environment::AddAgent(std::unique_ptr<Agent> agent) {
    // Save position/heading before move
    glm::vec2 pos = agent->GetPosition();
    float heading = agent->GetHeading();

    // Body definition
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_dynamicBody;
    bd.position = {pos.x, pos.y};
    bd.rotation = b2MakeRot(glm::radians(heading));

    // Create body in the world
    b2BodyId body = b2CreateBody(m_World, &bd);

    // Shape (circle for agent body)
    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = agent->GetTraits().bodyRadius;

    b2ShapeDef sd = b2DefaultShapeDef();
    sd.density = 1.0f;

    b2CreateCircleShape(body, &sd, &circle);

    // Give body to agent
    agent->SetBody(body);

    // Store agent in environment
    m_Agents.emplace_back(std::move(agent));
}
void Environment::AddResource(std::shared_ptr<IResource> r) {
    // TODO: box2d
    m_Resources.emplace_back(std::move(r));
}
void Environment::AddHome(std::shared_ptr<Home> h) {
    // TODO: box2d
    m_Homes.emplace_back(std::move(h));
}

const std::vector<glm::vec2>& Environment::GetBoundary() const {
    return m_Boundary;
}

void Environment::Update(float dt) {
    b2World_Step(m_World, dt, 4);

    // Ressourcen regenerieren
    for (auto& r : m_Resources) r->Regenerate(dt);

    // Sync and update agents
    for (auto& agent : m_Agents) {
        if (agent->GetBody().index1 != 0) {
            b2Transform xf = b2Body_GetTransform(agent->GetBody());
            glm::vec2 pos = {xf.p.x, xf.p.y};
            float angle = b2Rot_GetAngle(xf.q);

            agent->SetPosition(pos);
            agent->SetHeading(glm::degrees(angle));
        }
        agent->Update(dt, *this);
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

Home* Environment::GetNextFreeHome() {
    for (auto& h : m_Homes) {
        if (h->Occupancy() < h->GetMaxOccupancy()) {
            return h.get();
        }
    }
    return nullptr;
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
