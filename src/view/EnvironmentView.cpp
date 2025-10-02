#include <algorithm>
#include <gramma/view/EnvironmentView.hpp>

namespace gr {

EnvironmentView::EnvironmentView() {
}

void EnvironmentView::Init() {
    m_AgentBatchView.Init();
    m_Shader.BuildLine();
    m_Bounds.Init();
}

void EnvironmentView::SyncWithEnvironment(Environment* env) {
    if (!env) return;

    m_AgentBatchView.UpdateInstances(env->GetAgents());

    // --- FoodViews ---
    for (auto& foodPtr : env->GetFoodSources()) {
        FoodSource* food = foodPtr.get();
        if (m_FoodViews.find(food) == m_FoodViews.end()) {
            auto view = std::make_unique<FoodView>();
            view->Init();
            m_FoodViews[food] = std::move(view);
        }
    }
    for (auto it = m_FoodViews.begin(); it != m_FoodViews.end();) {
        bool stillExists = std::any_of(env->GetFoodSources().begin(), env->GetFoodSources().end(),
                                       [&](auto& fs) { return fs.get() == it->first; });
        if (!stillExists) {
            it = m_FoodViews.erase(it);
        } else {
            it++;
        }
    }
}

void EnvironmentView::Draw(Environment* env, const Camera2D& cam) {
    if (!env) return;

    auto vp = cam.ViewProj();

    // --- Bounds ---
    {
        std::vector<glm::vec2> contour = {{env->XMin(), env->YMin()},
                                          {env->XMax(), env->YMin()},
                                          {env->XMax(), env->YMax()},
                                          {env->XMin(), env->YMax()}};
        m_Bounds.Set(contour);
        m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }

    // --- Agents ---
    m_AgentBatchView.Draw(vp);

    // --- FoodSources zeichnen ---
    for (auto& foodPtr : env->GetFoodSources()) {
        FoodSource* food = foodPtr.get();
        auto it = m_FoodViews.find(food);
        if (it != m_FoodViews.end()) {
            it->second->Draw(food, vp);
        }
    }
}

}  // namespace gr
