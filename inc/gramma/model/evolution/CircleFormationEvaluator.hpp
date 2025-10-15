#pragma once
#include <gramma/model/particle/ParticleSystem.hpp>

namespace gr {

class CircleFormationEvaluator {
   public:
    CircleFormationEvaluator(float radius);
    void Evaluate(ParticleSystem& system);

   private:
    float m_Radius;
};

}  // namespace gr
