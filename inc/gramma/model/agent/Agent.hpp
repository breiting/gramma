#pragma once
#include <box2d/box2d.h>

#include <glm/vec2.hpp>
#include <gramma/model/agent/AgentTraits.hpp>
#include <gramma/model/need/INeed.hpp>
#include <gramma/model/task/ITask.hpp>
#include <memory>
#include <vector>

enum class AgentState { Idle, Executing, Dead, Rescued };

namespace gr {
class Environment;

class Agent {
   public:
    Agent();
    Agent(const std::string& id, const glm::vec2& pos, const glm::vec2& heading, std::unique_ptr<AgentTraits> traits);

    ~Agent();

    const std::string& GetId() const;
    void SetId(const std::string& id);

    /// Movement
    const glm::vec2& GetPosition() const;
    void SetPosition(const glm::vec2& p);

    const glm::vec2& GetHeading() const;
    void SetHeading(const glm::vec2& heading);

    void SetVelocity(const glm::vec2& v);
    glm::vec2 GetVelocity() const;

    /// Traits
    const AgentTraits& GetTraits() const;
    AgentTraits& GetTraits();
    void SetTraits(std::unique_ptr<AgentTraits> traits);
    template <typename T>
    T* GetTraitsAs();
    template <typename T>
    const T* GetTraitsAs() const;

    /// Needs
    const std::vector<std::unique_ptr<INeed>>& GetNeeds() const;

    /// State
    AgentState GetState() const;
    void SetState(AgentState state);

    /// Physics
    b2BodyId GetBody() const;
    void SetBody(b2BodyId body);

    /// Task
    void AssignTask(std::unique_ptr<ITask> t);
    void ClearTask();

    /// Decision + Update
    void Update(const Environment& env, float dt);

    void AddNeed(std::unique_ptr<INeed> need) {
        m_Needs.emplace_back(std::move(need));
    }

   private:
    void EvaluateNeeds(const Environment& env, float dt);

   private:
    std::string m_Id;
    glm::vec2 m_Position{0, 0};
    b2BodyId m_Body;
    glm::vec2 m_Heading{0, 1};  // north

    std::unique_ptr<AgentTraits> m_Traits;
    std::vector<std::unique_ptr<INeed>> m_Needs;
    std::unique_ptr<ITask> m_Task;

    AgentState m_State{AgentState::Idle};
};

}  // namespace gr
