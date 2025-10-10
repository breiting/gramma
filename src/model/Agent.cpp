#include <glm/glm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/task/ITask.hpp>
#include <gramma/model/task/TaskFactory.hpp>
#include <limits>

namespace gr {

Agent::Agent() {
}
Agent::Agent(const std::string& id, const glm::vec2& pos, const glm::vec2& heading, std::unique_ptr<AgentTraits> traits,
             Home* home)
    : m_Id(id), m_Position(pos), m_Heading(heading), m_Traits(std::move(traits)), m_Home(home) {
}

const std::string& Agent::GetId() const {
    return m_Id;
}

void Agent::SetId(const std::string& id) {
    m_Id = id;
}

void Agent::SetVelocity(const glm::vec2& v) {
    b2Body_SetLinearVelocity(m_Body, {v.x, v.y});
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

const glm::vec2& Agent::GetHeading() const {
    return m_Heading;
}
void Agent::SetHeading(const glm::vec2& deg) {
    m_Heading = deg;
}

const AgentTraits& Agent::GetTraits() const {
    return *m_Traits;
}

AgentTraits& Agent::GetTraits() {
    return *m_Traits;
}

void Agent::SetTraits(std::unique_ptr<AgentTraits> traits) {
    m_Traits = std::move(traits);
}

const std::vector<std::unique_ptr<INeed>>& Agent::GetNeeds() const {
    return m_Needs;
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

b2BodyId Agent::GetBody() const {
    return m_Body;
}
void Agent::SetBody(b2BodyId body) {
    m_Body = body;
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
