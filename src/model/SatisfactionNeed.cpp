#include <glm/glm.hpp>
#include <gramma/model/SatisfactionNeed.hpp>
#include <gramma/model/SegregationTraits.hpp>

namespace gr {

float SatisfactionNeed::Evaluate(const Agent& self, const Environment& env) const {
    const auto* traits = self.GetTraitsAs<SegregationTraits>();
    if (!traits) return 0.0f;

    // KD-Tree Query: Nachbarn innerhalb comfortRadius
    double query_pt[2] = {self.GetPosition().x, self.GetPosition().y};
    std::vector<nanoflann::ResultItem<size_t, double>> neighbors;
    nanoflann::SearchParameters params;
    env.GetKDTree().radiusSearch(query_pt, traits->comfortRadius * traits->comfortRadius, neighbors, params);

    int same = 0;
    int total = 0;
    for (auto& n : neighbors) {
        const Agent* other = env.GetAgent(n.first);
        if (!other || other == &self) continue;

        const auto* otherTraits = other->GetTraitsAs<SegregationTraits>();
        if (!otherTraits) continue;

        total++;
        if (otherTraits->group == traits->group) same++;
    }

    if (total == 0) return 0.0f;  // keine Nachbarn = unzufrieden

    float frac = static_cast<float>(same) / total;
    if (frac < traits->minSameFraction) {
        // Need hoch (0 = zufrieden, 1 = stark unzufrieden)
        return (traits->minSameFraction - frac) * m_Weight;
    }
    return 0.0f;  // zufrieden
}

}  // namespace gr
