#include <gramma/model/Agent.hpp>
#include <gramma/model/EnergyNeed.hpp>
#include <gramma/model/Environment.hpp>
#include <gramma/model/INeed.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/RestTask.hpp>
#include <gramma/model/SeekResourceTask.hpp>
#include <gramma/model/TaskFactory.hpp>
#include <gramma/model/Types.hpp>
#include <memory>

namespace gr {

std::unique_ptr<ITask> TaskFactory::MakeFor(const INeed& need, Agent& agent, const Environment& env) {
    // Utility-basierte, zentrale Abbildung Need -> Task (ohne if-else im Agent)
    if (need.Name() == "Energy") {
        // Regel: wenn Energie unter 'low' -> Ressource suchen, sonst zuhause ruhen
        const auto* e = dynamic_cast<const EnergyNeed*>(&need);
        float level = agent.GetEnergyLevel();
        if (e && level < e->LowThreshold()) {
            auto nearest = env.FindNearest(ResourceType::Food, agent.GetPosition());
            if (nearest) {
                return std::make_unique<SeekResourceTask>(nearest, e->IntakeGain(), e->Target());
            }
            // keine Ressource gefunden -> als Fallback ruhen (bringt wenig, aber blockiert nicht)
            return std::make_unique<RestTask>(e ? e->Target() : 0.85f);
        } else {
            float target = e ? e->Target() : 0.85f;
            return std::make_unique<RestTask>(target);
        }
    }

    // Default: nichts
    return {};
}

}  // namespace gr
