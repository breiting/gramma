#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/ISensor.hpp>
#include <gramma/model/Task.hpp>

namespace gr {

enum class AgentState {
    Idle,
    Executing,
};

/** Represents an agent (person) */
class Agent {
   public:
    Agent() = default;
    virtual ~Agent() = default;

    void AssignTask(TaskPtr task);
    void Update(float dt, const Room* room);
    void AttachSensor(SensorPtr sensor);
    const std::vector<SensorPtr>& GetSensors() const;

   public:
    glm::vec2 Position{0.0, 0.0};         // Current position (meters)
    glm::vec2 Velocity{0.0, 0.0};         // Current velocity (m/s)
    float Heading = 0.0f;                 // Orientation (0 = north, 90 = east, ...)
    AgentTraits Traits;                   // Agent traits (age, speed, ...)
    AgentState State = AgentState::Idle;  // The current state of the agent
    int chosenExitIndex;                  // TODO: remove

   private:
    TaskPtr m_CurrentTask;
    std::vector<SensorPtr> m_Sensors;
};

}  // namespace gr
