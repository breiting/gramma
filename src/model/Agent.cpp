#include <cmath>
#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/ITask.hpp>
#include <gramma/model/TaskFactory.hpp>
#include <limits>

namespace gr {

Agent::Agent(const glm::vec2& pos, float headingDeg, std::unique_ptr<AgentTraits> traits, Home* home)
    : m_Position(pos), m_HeadingDeg(headingDeg), m_Traits(std::move(traits)), m_Home(home) {
}

Agent::~Agent() = default;

void Agent::SetVelocity(const glm::vec2& v) {
    float len = glm::length(v);
    float maxV = m_Traits->maxSpeed;
    glm::vec2 clamped = (len > maxV && len > 1e-6f) ? (v / len) * maxV : v;
    m_Velocity = clamped;

    if (glm::length(m_Velocity) > 1e-6f) {
        m_HeadingDeg = glm::degrees(std::atan2(m_Velocity.x, m_Velocity.y));
    }
}

EnergyNeed* Agent::findEnergyNeed() const {
    for (auto& n : m_Needs) {
        if (auto* e = dynamic_cast<EnergyNeed*>(n.get())) return e;
    }
    return nullptr;
}

float Agent::GetEnergyLevel() const {
    if (auto* e = findEnergyNeed()) return e->Level();
    return 0.0f;
}

void Agent::AddEnergyIntake(float de) {
    if (auto* e = findEnergyNeed()) e->AddIntake(de);
}

void Agent::AddEnergyRest(float dt) {
    if (auto* e = findEnergyNeed()) e->AddRest(dt);
}

void Agent::AddActivityCost(float speed, float dt) {
    if (auto* e = findEnergyNeed()) e->AddActivityCost(speed, dt);
}

bool Agent::IsEnergyBelow(float t) const {
    return GetEnergyLevel() < t;
}

void Agent::AssignTask(std::unique_ptr<ITask> t) {
    m_Task = std::move(t);
    if (m_Task) {
        m_State = AgentState::Executing;
        m_Task->Start(*this);
    } else {
        m_State = AgentState::Idle;
    }
}

void Agent::ClearTask() {
    m_Task.reset();
    m_State = AgentState::Idle;
}

void Agent::EvaluateNeeds(const Environment& env, float dt) {
    // Needs updaten
    for (auto& n : m_Needs) n->Update(dt);

    // Tod prüfen: Energie = 0
    if (GetEnergyLevel() <= 0.0f) {
        m_State = AgentState::Dead;
        m_Velocity = {0, 0};
        return;
    }

    // Utility-Choice
    float bestU = -std::numeric_limits<float>::infinity();
    INeed* chosen = nullptr;
    for (auto& n : m_Needs) {
        float u = n->Utility(*this, env);  // default = Priority()
        if (u > bestU) {
            bestU = u;
            chosen = n.get();
        }
    }

    if (chosen && m_State != AgentState::Executing) {
        TaskFactory factory;
        AssignTask(factory.MakeFor(*chosen, *this, env));
    }
}

void Agent::Update(float dt, const Environment& env) {
    if (m_State == AgentState::Dead) return;

    EvaluateNeeds(env, dt);

    if (m_Task) {
        m_Task->Update(*this, dt);
        if (m_Task->IsFinished()) {
            ClearTask();
        }
    }

    // Kinematik (Euler)
    m_Position += m_Velocity * dt;

    // einfache Bounds (clamp in Environment-Bounds optional)
}

}  // namespace gr
