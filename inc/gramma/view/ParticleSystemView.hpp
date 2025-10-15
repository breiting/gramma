#pragma once

#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/HomeView.hpp>
#include <gramma/view/LineShape.hpp>
#include <gramma/view/ParticleView.hpp>

namespace gr {

/**
 * @brief Convenience facade that synchronizes a particle system and its renderer.
 */
class ParticleSystemView {
   public:
    ParticleSystemView();

    /** @brief Initialize GPU resources for bounds and particle rendering. */
    void Init();

    /**
     * @brief Upload instance data from the particle system.
     * @param system Optional particle system pointer; ignored if null.
     */
    void SyncWithParticleSystem(ParticleSystem* system);

    /**
     * @brief Render the synchronized particle instances.
     * @param system Particle system used for optional debug overlays.
     * @param cam Active 2D camera supplying the view/projection matrix.
     */
    void Draw(ParticleSystem* system, const Camera2D& cam);

   private:
    LineShape m_Bounds;
    Shader m_Shader;

    ParticleView m_ParticleView;
};

}  // namespace gr
