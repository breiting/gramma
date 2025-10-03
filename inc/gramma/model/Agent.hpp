#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/FoodSource.hpp>
#include <gramma/model/INeed.hpp>
#include <gramma/model/ISensor.hpp>
#include <gramma/model/Task.hpp>
#include <vector>

namespace gr {

enum class AgentState {
    Idle,
    Executing,
    Dead,
};

/** Represents an agent (acting entity) in the simulation */
class Agent {
   public:
    Agent(const glm::vec2& initialPosition, float headingDeg, std::unique_ptr<AgentTraits> traits);

    virtual ~Agent() = default;

    void AssignTask(TaskPtr task);

    // --- Update loops ---
    void Update(float dt, const Environment& env);
    void EvaluateNeeds(Environment& env, float dt);

    // --- Sensor Management ---
    void AttachSensor(SensorPtr sensor);
    const std::vector<SensorPtr>& GetSensors() const;

    // --- Need Management ---
    void AddNeed(std::unique_ptr<INeed> need);
    const std::vector<std::unique_ptr<INeed>>& GetNeeds() const;
    void SatisfyNeed(const std::string& need);

    // --- State Management ---
    void SetState(AgentState state);
    AgentState GetState() const;

    // --- Accessors ---
    const glm::vec2& GetPosition() const;
    void SetPosition(const glm::vec2& pos);

    float GetHeading() const;
    void SetHeading(float headingDeg);

    float GetSpeed() const;
    void SetSpeed(float speed);

    const glm::vec2& GetVelocity() const;  // derived from Heading + DesiredSpeed
    void SetVelocity(const glm::vec2& vel);

    const AgentTraits* GetTraits() const {
        return m_Traits.get();
    }
    AgentTraits* GetTraits() {
        return m_Traits.get();
    }

    template <typename T>
    T* GetTraitsAs() {
        return dynamic_cast<T*>(m_Traits.get());
    }

    template <typename T>
    const T* GetTraitsAs() const {
        return dynamic_cast<const T*>(m_Traits.get());
    }

   private:
    glm::vec2 m_Position;
    glm::vec2 m_Velocity;
    float m_Heading;  // in degrees, 0 = north
    float m_Speed;    // m/s

    std::unique_ptr<AgentTraits> m_Traits;
    AgentState m_State;

    TaskPtr m_CurrentTask;
    std::vector<SensorPtr> m_Sensors;
    std::vector<std::unique_ptr<INeed>> m_Needs;
};

}  // namespace gr
