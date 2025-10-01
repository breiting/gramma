#include <glm/glm.hpp>
#include <gramma/core/Environment.hpp>
#include <gramma/model/HungerNeed.hpp>
#include <gramma/model/RandomWalkTask.hpp>
#include <gramma/model/SeekFoodTask.hpp>
#include <gramma/model/Task.hpp>
#include <iomanip>
#include <iostream>

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

void Environment::Stats() const {
    size_t numAgents = m_Agents.size();
    size_t numFood = m_FoodSources.size();

    // Agents: Durchschnittlicher Hunger & Exercise
    float avgHunger = 0.0f;
    float avgExercise = 0.0f;
    int hungerCount = 0, exerciseCount = 0;

    for (const auto& a : m_Agents) {
        for (const auto& n : a->GetNeeds()) {
            if (n->Name() == "Hunger") {
                avgHunger += n->Priority();
                hungerCount++;
            } else if (n->Name() == "Exercise") {
                avgExercise += n->Priority();
                exerciseCount++;
            }
        }
    }
    if (hungerCount > 0) avgHunger /= hungerCount;
    if (exerciseCount > 0) avgExercise /= exerciseCount;

    // Food: Durchschnittliche Nutrition
    float avgFood = 0.0f;
    for (const auto& f : m_FoodSources) {
        avgFood += f->GetNutrition();
    }
    if (numFood > 0) avgFood /= numFood;

    // Ausgabe formatiert
    std::cout << "=== Environment Stats ===\n"
              << "Agents: " << numAgents << "  |  FoodSources: " << numFood << "\n"
              << "Avg Hunger:   " << std::fixed << std::setprecision(2) << avgHunger
              << "  |  Avg Exercise: " << avgExercise << "\n"
              << "Avg FoodNut:  " << avgFood << "\n"
              << "-------------------------\n";
}

void Environment::Update(float dt) {
    // Update FoodSources
    for (auto& f : m_FoodSources) {
        f->Regenerate(dt);
    }

    // Update Agents
    for (auto& a : m_Agents) {
        if (a->GetState() != AgentState::Dead) {
            a->EvaluateNeeds(m_FoodSources, dt);
            a->Update(dt);
        }
    }

    // Delete dead agents
    for (size_t i = 0; i < m_Agents.size();) {
        if (m_Agents[i]->GetState() == AgentState::Dead) {
            m_Agents.erase(m_Agents.begin() + i);
            m_AgentViews.erase(m_AgentViews.begin() + i);
        } else {
            ++i;
        }
    }

    // Delete empty food sources
    for (size_t i = 0; i < m_FoodSources.size();) {
        if (m_FoodSources[i]->GetNutrition() <= 0.0f) {
            m_FoodSources.erase(m_FoodSources.begin() + i);
            m_FoodViews.erase(m_FoodViews.begin() + i);
        } else {
            ++i;
        }
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
