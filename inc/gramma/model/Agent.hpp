#pragma once
#include <box2d/box2d.h>

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
    Agent(const std::string& id, const glm::vec2& pos, float headingDeg, std::unique_ptr<AgentTraits> traits,
          Home* home = nullptr);
    ~Agent();

    const std::string& GetId() const;

    void SetVelocity(const glm::vec2& v);
    glm::vec2 GetVelocity() const;

    const glm::vec2& GetPosition() const;
    void SetPosition(const glm::vec2& p);

    float GetHeading() const;
    void SetHeading(float deg);

    const AgentTraits& GetTraits() const;
    AgentTraits& GetTraits();

    template <typename T>
    T* GetTraitsAs();
    template <typename T>
    const T* GetTraitsAs() const;

    AgentState GetState() const;
    void SetState(AgentState state);

    // Need-Fassade (Energy)
    float GetEnergyLevel() const;
    void AddEnergyIntake(float de);
    void AddEnergyRest(float dt);
    void AddActivityCost(float speed, float dt);
    bool IsEnergyBelow(float t) const;

    b2BodyId GetBody() const {
        return m_Body;
    }
    void SetBody(b2BodyId body) {
        m_Body = body;
    }

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
    std::string m_Id;
    glm::vec2 m_Position{0, 0};
    b2BodyId m_Body;
    float m_HeadingDeg{0.0f};

    std::unique_ptr<AgentTraits> m_Traits;
    std::vector<std::unique_ptr<INeed>> m_Needs;
    std::unique_ptr<ITask> m_Task;

    AgentState m_State{AgentState::Idle};
    Home* m_Home{nullptr};
};

}  // namespace gr
