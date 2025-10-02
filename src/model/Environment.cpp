#include <glm/glm.hpp>
#include <gramma/model/Environment.hpp>
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
    m_Agents.push_back(std::move(agent));
}

void Environment::AddFoodSource(std::shared_ptr<FoodSource> food) {
    m_FoodSources.push_back(food);
}

void Environment::BuildSpatialIndex() {
    m_Cloud.agents = &m_Agents;
    if (!m_KDTree) {
        m_KDTree = std::make_unique<KDTreeType>(2, m_Cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
    }
    m_KDTree->buildIndex();
}

void Environment::Update(float dt) {
    // Update FoodSources
    for (auto& f : m_FoodSources) {
        f->Regenerate(dt);
    }

    // KD-Tree for agents
    BuildSpatialIndex();

    // Update Agents
    for (auto& a : m_Agents) {
        if (a->GetState() != AgentState::Dead) {
            a->EvaluateNeeds(*this, dt);
            a->Update(dt, *this);
        }
    }

    // Physics
    if (m_CollisionHandler) {
        m_CollisionHandler->Resolve(*this);
    }

    // Delete dead agents
    m_Agents.erase(std::remove_if(m_Agents.begin(), m_Agents.end(),
                                  [](const std::unique_ptr<Agent>& a) {
                                      return a->GetState() == AgentState::Dead;  //
                                  }),
                   m_Agents.end());

    // Delete empty food sources
    m_FoodSources.erase(std::remove_if(m_FoodSources.begin(), m_FoodSources.end(),
                                       [](std::shared_ptr<FoodSource>& f) {
                                           return f->GetNutrition() <= 0;  //
                                       }),
                        m_FoodSources.end());
}

const std::vector<std::unique_ptr<Agent>>& Environment::GetAgents() const {
    return m_Agents;
}

std::vector<std::shared_ptr<FoodSource>>& Environment::GetFoodSources() {
    return m_FoodSources;
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

}  // namespace gr
