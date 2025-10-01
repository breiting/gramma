#pragma once
#include <glm/mat4x4.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/FoodSource.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/FoodView.hpp>
#include <memory>
#include <vector>

namespace gr {

/**
 * Global environment that contains agents and food sources.
 * Responsible for updating the simulation state and rendering.
 */
class Environment {
   public:
    Environment(float xmin, float xmax, float ymin, float ymax);

    void AddAgent(std::unique_ptr<Agent> agent);
    void AddFoodSource(std::shared_ptr<FoodSource> food);

    void Update(float dt);
    void Render(const glm::mat4& vp);

    const std::vector<std::unique_ptr<Agent>>& GetAgents() const;
    std::vector<std::shared_ptr<FoodSource>> GetFoodSources();

    float XMin() const {
        return m_Xmin;
    }
    float XMax() const {
        return m_Xmax;
    }
    float YMin() const {
        return m_Ymin;
    }
    float YMax() const {
        return m_Ymax;
    }

   private:
    float m_Xmin, m_Xmax, m_Ymin, m_Ymax;

    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::vector<std::shared_ptr<FoodSource>> m_FoodSources;

    // Views
    std::vector<std::unique_ptr<AgentView>> m_AgentViews;
    std::vector<std::unique_ptr<FoodView>> m_FoodViews;
};

}  // namespace gr
