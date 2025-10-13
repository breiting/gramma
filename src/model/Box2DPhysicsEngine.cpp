#include <box2d/box2d.h>

#include <gramma/core/Math.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/physics/Box2DPhysicsEngine.hpp>
#include <iostream>
#include <utility>

namespace gr {

Box2DPhysicsEngine::Box2DPhysicsEngine() {
    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = {0.0, 0.0};  // no gravity
    m_World = b2CreateWorld(&def);
}

Box2DPhysicsEngine::~Box2DPhysicsEngine() {
    b2DestroyWorld(m_World);
}

void Box2DPhysicsEngine::Init() {
    // nothing for now
}

void Box2DPhysicsEngine::Step(float dt) {
    b2World_Step(m_World, dt, 4);
}

void Box2DPhysicsEngine::AddBoundary(const std::vector<glm::vec2>& contour) {
    CreateChainShape(contour);
}

void Box2DPhysicsEngine::AddObstacle(const std::vector<glm::vec2>& contour) {
    CreateChainShape(contour);
}

void Box2DPhysicsEngine::AddResource(std::shared_ptr<IResource> r) {
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
}

void Box2DPhysicsEngine::RemoveResource(IResource* r) {
    b2DestroyBody(r->GetBody());
}

void Box2DPhysicsEngine::AddAgent(Agent& agent) {
    glm::vec2 pos = agent.GetPosition();
    float mass = agent.GetTraits().mass;
    float radius = agent.GetTraits().bodyRadius;

    // Body definition
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_dynamicBody;
    bd.position = {pos.x, pos.y};

    // Create body in the world
    b2BodyId body = b2CreateBody(m_World, &bd);
    b2Body_SetFixedRotation(body, true);

    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = agent.GetTraits().bodyRadius;

    b2ShapeDef sd = b2DefaultShapeDef();
    sd.density = mass / (radius * radius * gr::PI);  // kg/m^2

    b2SurfaceMaterial mat = b2DefaultSurfaceMaterial();
    mat.friction = 0.3;
    mat.restitution = 0.2;
    sd.material = mat;

    b2CreateCircleShape(body, &sd, &circle);
}

void Box2DPhysicsEngine::RemoveAgent(Agent& agent) {
    auto it = m_Bodies.find(&agent);
    if (it != m_Bodies.end()) {
        b2DestroyBody(it->second);
        m_Bodies.erase(it);
    }
}

void Box2DPhysicsEngine::SyncAgentState(Agent& agent) {
    if (auto it = m_Bodies.find(&agent); it != m_Bodies.end()) {
        b2Transform xf = b2Body_GetTransform(it->second);
        glm::vec2 pos = {xf.p.x, xf.p.y};
        agent.SetPosition(pos);
    }
}

void Box2DPhysicsEngine::ApplyMovement(Agent& agent, const glm::vec2& movement) {
    if (auto it = m_Bodies.find(&agent); it != m_Bodies.end()) {
        b2Body_SetLinearVelocity(it->second, {movement.x, movement.y});
        if (glm::length(movement) > 0.0) {
            agent.SetHeading(glm::normalize(movement));
        }
    }
}

std::vector<const Agent*> Box2DPhysicsEngine::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    std::vector<const Agent*> results;

    b2AABB aabb;
    aabb.lowerBound = {pos.x - radius, pos.y - radius};
    aabb.upperBound = {pos.x + radius, pos.y + radius};

    auto callback = [](b2ShapeId shapeId, void* context) -> bool {
        auto* data =
            static_cast<std::tuple<const Environment*, glm::vec2, float, std::vector<const Agent*>*>*>(context);
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
    auto context = std::make_tuple(this, pos, radius, &results);

    b2World_OverlapAABB(m_World, aabb, filter, callback, &context);

    return results;
}

void Box2DPhysicsEngine::CreateChainShape(const std::vector<glm::vec2>& contour) {
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

}  // namespace gr
