#pragma once

#include <gramma/core/IApp.hpp>
#include <gramma/model/evolution/CircleFormationEvaluator.hpp>
#include <gramma/model/evolution/Population.hpp>
#include <gramma/model/particle/ParticleSystem.hpp>
#include <gramma/ui/IGuiLayer.hpp>
#include <gramma/view/Camera2D.hpp>
#include <gramma/view/ParticleSystemView.hpp>

/**
 * @brief Standalone demo app showcasing evolution
 */
class EvolutionApp : public gr::IApp {
   public:
    ~EvolutionApp() = default;
    std::string Name() const override;

    bool Init(gr::AppContext& ctx) override;
    void Update(gr::AppContext& ctx, double dt) override;
    void Render(gr::AppContext& ctx) override;

   private:
    gr::Camera2D m_Camera;

    std::unique_ptr<gr::ParticleSystem> m_System;
    gr::ParticleSystemView m_View;

    std::unique_ptr<gr::CircleFormationEvaluator> m_Evaluator;
    std::unique_ptr<gr::Population> m_Population;

    std::unique_ptr<gr::IGuiLayer> m_Gui;

    glm::vec2 m_MousePos{0, 0};
    glm::vec2 m_DragStart{0, 0};
    bool m_IsDragging = false;

    int m_Generation{0};
    int m_GenerationSteps = 500;
    int m_CurrentStep = 0;
    bool m_Running = true;

    float m_Timescale = 1.0f;
    float m_GlobalWidth = 200.0f;
    float m_GlobalHeight = 200.0f;

    float m_CellSize = 3.0f;
    float m_NeighborhoodRadius = 5.0f;
    float m_ParticleRadius = 0.8f;

    int m_NumGroups = 1;
    int m_NumParticles = 500;

    bool m_Quit{false};
};
