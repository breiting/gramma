

#pragma once
#include <gramma/model/INeed.hpp>
#include <string>

namespace gr {

struct EnergyNeedParams {
    float basal = 0.003f;      // Energy consumption during rest (per seconds)
    float kActivity = 0.006f;  // Energy consumption during activity
    float p = 2.0f;            // Exponent (linear/quadric)
    float restGain = 0.001f;   // Regeneration energy
    float intakeGain = 0.15f;  // Energy gain during eating
    float low = 0.35f;         // Threshold for hunger
    float target = 0.85f;      // Target energy
};

class EnergyNeed : public INeed {
   public:
    explicit EnergyNeed(const EnergyNeedParams& p = EnergyNeedParams{});

    void Update(float dt) override;
    float Priority() const override {
        return 1.0f - m_E;
    }
    std::string Name() const override {
        return "Energy";
    }

    void AddIntake(float e);
    void AddRest(float dt);
    void AddActivityCost(float speed, float dt);

    float Level() const {
        return m_E;
    }
    float LowThreshold() const {
        return m_P.low;
    }
    float Target() const {
        return m_P.target;
    }
    float IntakeGain() const {
        return m_P.intakeGain;
    }

   private:
    EnergyNeedParams m_P;
    float m_E = 0.8f;
};

}  // namespace gr
