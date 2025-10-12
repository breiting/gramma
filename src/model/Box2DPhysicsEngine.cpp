#include <box2d/box2d.h>

#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/physics/Box2DPhysicsEngine.hpp>

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

    // register user data
    b2Body_SetUserData(body, &agent);
    m_Bodies[&agent] = body;
    agent.SetBody(body);
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
        b2Vec2 p = it->second->GetPosition();
        float angle = it->second->GetAngle();
        agent.SetPosition({p.x, p.y});
        agent.SetHeading({sinf(angle), cosf(angle)});
    }
}

void Box2DPhysicsEngine::ApplyMovement(Agent& agent, const glm::vec2& movement) {
    if (auto it = m_Bodies.find(&agent); it != m_Bodies.end()) {
        it->second->SetLinearVelocity({movement.x, movement.y});
    }
}

std::vector<Agent*> Box2DPhysicsEngine::QueryAgentsInRadius(const glm::vec2& pos, float radius) const {
    // Optional: You could use b2World::QueryAABB here, for now just return everything
    std::vector<Agent*> result;
    for (auto& [agent, body] : m_Bodies) {
        b2Vec2 p = body->GetPosition();
        if (glm::length(glm::vec2{p.x, p.y} - pos) <= radius) result.push_back(agent);
    }
    return result;
}

}  // namespace gr
