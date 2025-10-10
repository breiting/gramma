#include <algorithm>
#include <gramma/view/EnvironmentView.hpp>

#include "gramma/model/resource/IResource.hpp"

namespace gr {

EnvironmentView::EnvironmentView() {
}

void EnvironmentView::Init() {
    m_AgentView.Init();
    m_Shader.BuildLine();
    m_Bounds.Init();
}

void EnvironmentView::SyncWithEnvironment(Environment* env) {
    if (!env) return;

    m_AgentView.UpdateInstances(env->Agents());

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

    // --- Draw Bounds ---
    {
        m_Bounds.Set(env->GetBoundary());
        m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

        for (auto& o : env->GetObstacles()) {
            m_Bounds.Set(o);
            m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
        }
    }

    // --- Draw Agents ---
    m_AgentView.Draw(vp);

    // --- Draw Resources ---
    for (auto& rPtr : env->Resources()) {
        IResource* res = rPtr.get();
        auto it = m_ResourceViews.find(res);
        if (it != m_ResourceViews.end()) {
            it->second->Draw(res, vp);
        }
    }
}

}  // namespace gr
