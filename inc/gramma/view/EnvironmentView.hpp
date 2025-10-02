#pragma once
#include <gramma/model/Environment.hpp>
#include <gramma/view/AgentView.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/FoodView.hpp>
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

    std::unordered_map<Agent*, std::unique_ptr<AgentView>> m_AgentViews;
    std::unordered_map<FoodSource*, std::unique_ptr<FoodView>> m_FoodViews;
};

}  // namespace gr
