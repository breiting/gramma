#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Room.hpp>

namespace gr {

Agent::Agent(const glm::vec2& initialPosition, float headingDeg, const AgentTraits& traits)
    : m_Position(initialPosition),
      m_Velocity(0.0f, 0.0f),
      m_Heading(headingDeg),
      m_DesiredSpeed(0.0f),
      m_Traits(traits),
      m_State(AgentState::Idle) {
}

void Agent::AssignTask(TaskPtr task) {
    m_CurrentTask = std::move(task);
    SetState(AgentState::Executing);
    if (m_CurrentTask) {
        m_CurrentTask->Start(*this);
    }
}

void Agent::Update(float dt, const Room* room) {
    // Sensors
    for (auto& s : m_Sensors) {
        s->Update(*this, *room);
    }

    // Task
    if (m_State == AgentState::Executing && m_CurrentTask) {
        m_CurrentTask->Update(*this, dt);
        if (m_CurrentTask->IsFinished()) {
            SetState(AgentState::Idle);
            m_CurrentTask.reset();
        }
    }

    // Kinematics
    UpdateKinematics(dt);
}

void Agent::AttachSensor(SensorPtr sensor) {
    m_Sensors.push_back(std::move(sensor));
}

const std::vector<SensorPtr>& Agent::GetSensors() const {
    return m_Sensors;
}

// --- State ---
void Agent::SetState(AgentState state) {
    m_State = state;
}
AgentState Agent::GetState() const {
    return m_State;
}

// --- Position ---
const glm::vec2& Agent::GetPosition() const {
    return m_Position;
}
void Agent::SetPosition(const glm::vec2& pos) {
    m_Position = pos;
}

// --- Heading ---
float Agent::GetHeading() const {
    return m_Heading;
}
void Agent::SetHeading(float headingDeg) {
    m_Heading = headingDeg;
}

// --- Desired Speed ---
float Agent::GetDesiredSpeed() const {
    return m_DesiredSpeed;
}
void Agent::SetDesiredSpeed(float speed) {
    m_DesiredSpeed = speed;
}

// --- Velocity ---
const glm::vec2& Agent::GetVelocity() const {
    return m_Velocity;
}

// --- Traits ---
const AgentTraits& Agent::GetTraits() const {
    return m_Traits;
}
void Agent::SetTraits(const AgentTraits& traits) {
    m_Traits = traits;
}

// --- Private ---
void Agent::UpdateKinematics(float dt) {
    float angleRad = glm::radians(m_Heading);
    m_Velocity = glm::vec2(std::sin(angleRad), std::cos(angleRad)) * m_DesiredSpeed;
    m_Position += m_Velocity * dt;
}

}  // namespace gr
