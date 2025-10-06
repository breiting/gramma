

#pragma once
#include <gramma/model/INeed.hpp>
#include <string>

namespace gr {

struct EnergyNeedParams {
    float basal = 0.002f;       // Grundverbrauch pro Sekunde
    float kActivity = 0.004f;   // Faktor für Aktivitätskosten
    float p = 2.0f;             // Exponent (linear/quadratisch)
    float restGain = 0.003f;    // Regeneration beim Ausruhen
    float intakeGain = 0.015f;  // Zuwachs pro Sekunde beim Essen
    float low = 0.35f;          // Schwelle für Hunger
    float target = 0.85f;       // Zielenergie
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

    // Agent-API ruft diese:
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
