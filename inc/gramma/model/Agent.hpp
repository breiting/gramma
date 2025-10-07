#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/AgentTraits.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/INeed.hpp>
#include <gramma/model/ITask.hpp>
#include <gramma/model/Types.hpp>
#include <memory>
#include <vector>

namespace gr {
class Environment;
class Home;

class Agent {
   public:
    Agent(const glm::vec2& pos, float headingDeg, std::unique_ptr<AgentTraits> traits, Home* home = nullptr);
    ~Agent();

    const glm::vec2& GetPosition() const {
        return m_Position;
    }
    void SetPosition(const glm::vec2& p) {
        m_Position = p;
    }

    const glm::vec2& GetVelocity() const {
        return m_Velocity;
    }
    void SetVelocity(const glm::vec2& v);

    float GetHeading() const {
        return m_HeadingDeg;
    }
    void SetHeading(float deg) {
        m_HeadingDeg = deg;
    }

    const AgentTraits& GetTraits() const {
        return *m_Traits;
    }
    AgentTraits& GetTraits() {
        return *m_Traits;
    }

    template <typename T>
    T* GetTraitsAs() {
        return dynamic_cast<T*>(m_Traits.get());
    }
    template <typename T>
    const T* GetTraitsAs() const {
        return dynamic_cast<const T*>(m_Traits.get());
    }

    AgentState GetState() const {
        return m_State;
    }

    // Need-Fassade (Energy)
    float GetEnergyLevel() const;
    void AddEnergyIntake(float de);
    void AddEnergyRest(float dt);
    void AddActivityCost(float speed, float dt);
    bool IsEnergyBelow(float t) const;

    // Task
    void AssignTask(std::unique_ptr<ITask> t);
    void ClearTask();

    // Decision + Update
    void Update(float dt, const Environment& env);
    void EvaluateNeeds(const Environment& env, float dt);

    // Home
    Home* GetHome() const {
        return m_Home;
    }
    void SetHome(Home* h) {
        m_Home = h;
    }

    void AddNeed(std::unique_ptr<INeed> need) {
        m_Needs.emplace_back(std::move(need));
    }

   private:
    EnergyNeed* findEnergyNeed() const;

   private:
    glm::vec2 m_Position{0, 0};
    glm::vec2 m_Velocity{0, 0};
    float m_HeadingDeg{0.0f};

    std::unique_ptr<AgentTraits> m_Traits;
    std::vector<std::unique_ptr<INeed>> m_Needs;
    std::unique_ptr<ITask> m_Task;

    AgentState m_State{AgentState::Idle};
    Home* m_Home{nullptr};
};

}  // namespace gr
