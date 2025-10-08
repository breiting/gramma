#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/ITask.hpp>
#include <gramma/model/TaskFactory.hpp>
#include <limits>

#include "gramma/model/Types.hpp"

namespace gr {

Agent::Agent(const std::string& id, const glm::vec2& pos, float headingDeg, std::unique_ptr<AgentTraits> traits,
             Home* home)
    : m_Id(id), m_Position(pos), m_HeadingDeg(headingDeg), m_Traits(std::move(traits)), m_Home(home) {
}

const std::string& Agent::GetId() const {
    return m_Id;
}

void Agent::SetVelocity(const glm::vec2& v) {
    b2Body_SetLinearVelocity(m_Body, {v.x, v.y});
    // for human like movement
    // b2Body_ApplyForceToCenter(m_Body, {v.x, v.y}, true);
}

glm::vec2 Agent::GetVelocity() const {
    if (m_Body.index1) {
        auto v = b2Body_GetLinearVelocity(m_Body);
        return glm::vec2(v.x, v.y);
    }
    return glm::vec2{0};
}

const glm::vec2& Agent::GetPosition() const {
    return m_Position;
}
void Agent::SetPosition(const glm::vec2& p) {
    m_Position = p;
}

float Agent::GetHeading() const {
    return m_HeadingDeg;
}
void Agent::SetHeading(float deg) {
    m_HeadingDeg = deg;
}

const AgentTraits& Agent::GetTraits() const {
    return *m_Traits;
}
AgentTraits& Agent::GetTraits() {
    return *m_Traits;
}

template <typename T>
T* Agent::GetTraitsAs() {
    return dynamic_cast<T*>(m_Traits.get());
}
template <typename T>
const T* Agent::GetTraitsAs() const {
    return dynamic_cast<const T*>(m_Traits.get());
}

AgentState Agent::GetState() const {
    return m_State;
}

void Agent::SetState(AgentState state) {
    m_State = state;
}

Agent::~Agent() = default;

EnergyNeed* Agent::findEnergyNeed() const {
    for (auto& n : m_Needs) {
        if (auto* e = dynamic_cast<EnergyNeed*>(n.get())) {
            return e;
        }
    }
    return nullptr;
}

float Agent::GetEnergyLevel() const {
    if (auto* e = findEnergyNeed()) {
        return e->Level();
    }
    return 0.0f;
}

void Agent::AddEnergyIntake(float de) {
    if (auto* e = findEnergyNeed()) {
        e->AddIntake(de);
    }
}

void Agent::AddEnergyRest(float dt) {
    if (auto* e = findEnergyNeed()) {
        e->AddRest(dt);
    }
}

void Agent::AddActivityCost(float speed, float dt) {
    if (auto* e = findEnergyNeed()) {
        e->AddActivityCost(speed, dt);
    }
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
}

void Agent::EvaluateNeeds(const Environment& env, float dt) {
    for (auto& n : m_Needs) {
        n->Update(dt);
    }

    // TODO:
    // if (GetEnergyLevel() <= 0.0f) {
    //     m_State = AgentState::Dead;
    //     return;
    // }

    // Utility-Choice
    float bestU = -std::numeric_limits<float>::infinity();
    INeed* chosen = nullptr;
    for (auto& n : m_Needs) {
        float u = n->Utility(*this, env);
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
    if (m_State == AgentState::Dead || m_State == AgentState::Rescued) return;

    EvaluateNeeds(env, dt);

    if (m_Task) {
        m_Task->Update(*this, dt);
        if (m_Task->IsFinished()) {
            ClearTask();
        }
    }
}

}  // namespace gr
