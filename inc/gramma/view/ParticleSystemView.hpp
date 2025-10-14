#pragma once
#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/HomeView.hpp>
#include <gramma/view/LineShape.hpp>
#include <gramma/view/ParticleView.hpp>

namespace gr {

/**
 * View for ParticleSystem
 */
class ParticleSystemView {
   public:
    ParticleSystemView();

    void Init();

    void SyncWithParticleSystem(ParticleSystem* system);

    void Draw(ParticleSystem* system, const Camera2D& cam);

   private:
    LineShape m_Bounds;
    Shader m_Shader;

    ParticleView m_ParticleView;
};

}  // namespace gr
