#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/environment/Environment.hpp>
#include <gramma/model/task/ITask.hpp>
#include <gramma/model/task/TaskFactory.hpp>
#include <iomanip>
#include <iostream>
#include <limits>

namespace gr {

Agent::Agent() {
}
Agent::Agent(const std::string& id, const glm::vec2& pos, const glm::vec2& heading, std::unique_ptr<AgentTraits> traits)
    : m_Id(id), m_Position(pos), m_Heading(heading), m_Traits(std::move(traits)) {
}

const std::string& Agent::GetId() const {
    return m_Id;
}

void Agent::SetId(const std::string& id) {
    m_Id = id;
}

void Agent::SetVelocity(const glm::vec2& v) {
    b2Body_SetLinearVelocity(m_Body, {v.x, v.y});
    if (glm::length(v) > 0.0) {
        m_Heading = glm::normalize(v);
    }
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
void Agent::SetHeading(const glm::vec2& head) {
    m_Heading = head;
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
    // 1. Update Needs
    for (auto& n : m_Needs) {
        n->Update(dt);
    }

    // 2. Utility
    float bestU = -std::numeric_limits<float>::infinity();
    INeed* chosen = nullptr;

    for (auto& n : m_Needs) {
        float u = n->Utility(*this, env);
        if (u > bestU) {
            bestU = u;
            chosen = n.get();
        }
    }

    if (chosen && (!m_Task || m_Task->IsFinished())) {
        auto& factory = TaskFactory::Instance();
        auto newTask = factory.BuildTaskForNeed(*chosen, *this, env);

        if (newTask) {
            AssignTask(std::move(newTask));
            m_State = AgentState::Executing;
        }
    }
}

void Agent::Update(const Environment& env, float dt) {
    if (m_State == AgentState::Dead || m_State == AgentState::Rescued) return;

    EvaluateNeeds(env, dt);

    if (m_Task) {
        m_Task->Update(*this, dt);
        if (m_Task->IsFinished()) {
            ClearTask();
        }
    }
}

void Agent::Print() const {
    std::cout << "Agent: " << m_Id << "\n";
    std::cout << "  Position: (" << m_Position.x << ", " << m_Position.y << ")\n";
    std::cout << "  Heading : (" << m_Heading.x << ", " << m_Heading.y << ")\n";

    std::cout << "  State   : ";
    switch (m_State) {
        case AgentState::Idle:
            std::cout << "Idle";
            break;
        case AgentState::Executing:
            std::cout << "Executing";
            break;
        case AgentState::Dead:
            std::cout << "Dead";
            break;
        case AgentState::Rescued:
            std::cout << "Rescued";
            break;
        default:
            std::cout << "Unknown";
            break;
    }
    std::cout << "\n";

    if (m_Traits) {
        std::cout << "  Traits:\n";
        std::cout << "    Age Class   : ";
        switch (m_Traits->age) {
            case AgeClass::Child:
                std::cout << "Child";
                break;
            case AgeClass::Teenager:
                std::cout << "Teenager";
                break;
            case AgeClass::Adult:
                std::cout << "Adult";
                break;
            case AgeClass::Senior:
                std::cout << "Senior";
                break;
        }
        std::cout << "\n";

        std::cout << "    Sex         : " << (m_Traits->sex == Sex::Male ? "Male" : "Female") << "\n";
        std::cout << "    Mass        : " << m_Traits->mass << " kg\n";
        std::cout << "    BodyRadius  : " << m_Traits->bodyRadius << " m\n";
        std::cout << "    SocialRadius: " << m_Traits->socialRadius << " m\n";
        std::cout << "    MaxSpeed    : " << m_Traits->maxSpeed << " m/s\n";
    }

    if (!m_Needs.empty()) {
        std::cout << "  Needs:\n";
        for (const auto& n : m_Needs) {
            std::cout << "    - " << n->Name() << " | Priority: " << std::fixed << std::setprecision(2) << n->Priority()
                      << "\n";
        }
    }

    std::cout << std::endl;
}

}  // namespace gr
