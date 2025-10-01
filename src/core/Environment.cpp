#include <glm/glm.hpp>
#include <gramma/core/Environment.hpp>
#include <gramma/model/HungerNeed.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/SeekFoodTask.hpp>
#include <gramma/model/Task.hpp>

namespace gr {

Environment::Environment(float xmin, float xmax, float ymin, float ymax)
    : m_Xmin(xmin), m_Xmax(xmax), m_Ymin(ymin), m_Ymax(ymax) {
}

void Environment::AddAgent(std::unique_ptr<Agent> agent) {
    auto view = std::make_unique<AgentView>();
    view->Init();

    m_AgentViews.push_back(std::move(view));
    m_Agents.push_back(std::move(agent));
}

void Environment::AddFoodSource(std::shared_ptr<FoodSource> food) {
    m_FoodSources.push_back(food);

    auto view = std::make_unique<FoodView>();
    view->Init();
    m_FoodViews.push_back(std::move(view));
}

void Environment::Update(float dt) {
    // Update FoodSources
    for (auto& f : m_FoodSources) {
        f->Regenerate(dt);
    }

    // Update Agents
    for (auto& a : m_Agents) {
        a->EvaluateNeeds(m_FoodSources, dt);
        a->Update(dt);
    }
}

void Environment::Render(const glm::mat4& vp) {
    // FoodSources
    for (size_t i = 0; i < m_FoodSources.size(); ++i) {
        m_FoodViews[i]->Draw(m_FoodSources[i].get(), vp);
    }

    // Agents
    for (size_t i = 0; i < m_Agents.size(); ++i) {
        m_AgentViews[i]->Draw(m_Agents[i].get(), vp);
    }
}

const std::vector<std::unique_ptr<Agent>>& Environment::GetAgents() const {
    return m_Agents;
}

std::vector<std::shared_ptr<FoodSource>> Environment::GetFoodSources() {
    return m_FoodSources;
}

}  // namespace gr
