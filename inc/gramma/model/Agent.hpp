#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/FoodSource.hpp>
#include <gramma/model/INeed.hpp>
#include <gramma/model/ISensor.hpp>
#include <gramma/model/Task.hpp>
#include <vector>

namespace gr {

class Room;

enum class AgentState {
    Idle,
    Executing,
};

/** Represents an agent (person) in the simulation */
class Agent {
   public:
    Agent(const glm::vec2& initialPosition, float headingDeg, const AgentTraits& traits);

    virtual ~Agent() = default;

    // --- Simulation API ---
    void AssignTask(TaskPtr task);
    void Update(float dt, const Room* room);

    // --- Sensor Management ---
    void AttachSensor(SensorPtr sensor);
    const std::vector<SensorPtr>& GetSensors() const;

    // --- Need Management ---
    void AddNeed(std::unique_ptr<INeed> need);
    void EvaluateNeeds(const std::vector<FoodSource>& foodSources);

    // --- State Management ---
    void SetState(AgentState state);
    AgentState GetState() const;

    // --- Accessors ---
    const glm::vec2& GetPosition() const;
    void SetPosition(const glm::vec2& pos);

    float GetHeading() const;
    void SetHeading(float headingDeg);

    float GetDesiredSpeed() const;
    void SetDesiredSpeed(float speed);

    const glm::vec2& GetVelocity() const;  // derived from Heading + DesiredSpeed
    const AgentTraits& GetTraits() const;
    void SetTraits(const AgentTraits& traits);

   private:
    void UpdateKinematics(float dt);

   private:
    glm::vec2 m_Position;
    glm::vec2 m_Velocity;
    float m_Heading;       // in degrees, 0 = north
    float m_DesiredSpeed;  // m/s

    AgentTraits m_Traits;
    AgentState m_State;

    TaskPtr m_CurrentTask;
    std::vector<SensorPtr> m_Sensors;
    std::vector<std::unique_ptr<INeed>> m_Needs;
};

}  // namespace gr
