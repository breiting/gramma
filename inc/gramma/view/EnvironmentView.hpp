#pragma once
#include <gramma/model/Environment.hpp>
#include <gramma/view/AgentBatchView.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/FoodView.hpp>
#include <gramma/view/HomeView.hpp>
#include <gramma/view/LineShape.hpp>
#include <memory>
#include <unordered_map>

namespace gr {

/**
 * View for Environment
 */
class EnvironmentView {
   public:
    EnvironmentView();

    void Init();

    void SyncWithEnvironment(Environment* env);

    void Draw(Environment* env, const Camera2D& cam);

   private:
    LineShape m_Bounds;
    Shader m_Shader;

    AgentBatchView m_AgentBatchView;
    std::unordered_map<FoodSource*, std::unique_ptr<FoodView>> m_FoodViews;
    std::unordered_map<Home*, std::unique_ptr<HomeView>> m_HomeViews;
};

}  // namespace gr
