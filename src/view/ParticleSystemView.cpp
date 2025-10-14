#include <gramma/view/ParticleSystemView.hpp>

namespace gr {

ParticleSystemView::ParticleSystemView() {
}

void ParticleSystemView::Init() {
    m_ParticleView.Init();
    m_Shader.BuildLine();
    m_Bounds.Init();
}

void ParticleSystemView::SyncWithParticleSystem(ParticleSystem* system) {
    if (!system) return;

    m_ParticleView.UpdateInstances(system->GetParticles());
}

void ParticleSystemView::Draw(ParticleSystem* system, const Camera2D& cam) {
    if (!system) return;

    auto vp = cam.ViewProj();

    // --- Draw Bounds ---
    // {
    //     m_Bounds.Set(system->GetBoundary());
    //     m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    //
    //     for (auto& o : system->GetObstacles()) {
    //         m_Bounds.Set(o);
    //         m_Bounds.Draw(m_Shader, cam.ViewProj(), glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
    //     }
    // }

    // --- Draw Particles ---
    m_ParticleView.Draw(vp);
}

}  // namespace gr
