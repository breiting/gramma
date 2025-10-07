#include <algorithm>
#include <gramma/view/EnvironmentView.hpp>

#include "gramma/model/IResource.hpp"

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

    m_AgentBatchView.UpdateInstances(env->Agents());

    // --- Homes ---
    for (auto& homePtr : env->Homes()) {
        Home* home = homePtr.get();
        if (m_HomeViews.find(home) == m_HomeViews.end()) {
            auto view = std::make_unique<HomeView>();
            view->Init();
            m_HomeViews[home] = std::move(view);
        }
    }
    // --- ResourceViews ---
    for (auto& rPtr : env->Resources()) {
        IResource* res = rPtr.get();
        if (m_ResourceViews.find(res) == m_ResourceViews.end()) {
            auto view = std::make_unique<ResourceView>();
            view->Init();
            m_ResourceViews[res] = std::move(view);
        }
    }
    for (auto it = m_ResourceViews.begin(); it != m_ResourceViews.end();) {
        bool stillExists = std::any_of(env->Resources().begin(), env->Resources().end(),
                                       [&](auto& fs) { return fs.get() == it->first; });
        if (!stillExists) {
            it = m_ResourceViews.erase(it);
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
        m_Bounds.Set(env->GetBoundary());
        m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }

    // --- Agents ---
    m_AgentBatchView.Draw(vp);

    // --- Draw Resources ---
    for (auto& rPtr : env->Resources()) {
        IResource* res = rPtr.get();
        auto it = m_ResourceViews.find(res);
        if (it != m_ResourceViews.end()) {
            it->second->Draw(res, vp);
        }
    }

    // --- Draw Homes ---
    for (auto& homePtr : env->Homes()) {
        Home* home = homePtr.get();
        auto it = m_HomeViews.find(home);
        if (it != m_HomeViews.end()) {
            it->second->Draw(home, vp);
        }
    }
}

}  // namespace gr
