#include <algorithm>
#include <gramma/model/evolution/Population.hpp>
#include <numeric>

namespace gr {

Population::Population(size_t size, std::vector<int> topology, ActivationType act)
    : m_Topology(std::move(topology)), m_ActType(act), m_Rng(std::random_device{}()) {
    m_Genomes.reserve(size);
    m_Fitness.resize(size, 0.0f);
    for (size_t i = 0; i < size; ++i) {
        auto g = std::make_unique<FeedForwardGenome>(m_Topology, m_ActType);
        g->Randomize();
        m_Genomes.push_back(std::move(g));
    }
}

void Population::SetFitness(const std::vector<float>& fitness) {
    if (fitness.size() != m_Genomes.size()) throw std::runtime_error("Fitness size mismatch");
    m_Fitness = fitness;
}

const FeedForwardGenome& Population::GetBestGenome() const {
    auto it = std::max_element(m_Fitness.begin(), m_Fitness.end());
    return *m_Genomes[std::distance(m_Fitness.begin(), it)];
}

void Population::Evolve() {
    const size_t N = m_Genomes.size();
    std::vector<size_t> sortedIndices(N);
    std::iota(sortedIndices.begin(), sortedIndices.end(), 0);

    std::sort(sortedIndices.begin(), sortedIndices.end(),
              [&](size_t a, size_t b) { return m_Fitness[a] > m_Fitness[b]; });

    const size_t survivors = N / 4;  // top 25%
    std::vector<std::unique_ptr<FeedForwardGenome>> nextGen;

    for (size_t i = 0; i < survivors; ++i) {
        nextGen.push_back(std::make_unique<FeedForwardGenome>(*m_Genomes[sortedIndices[i]]));
    }

    std::uniform_int_distribution<size_t> pick(0, survivors - 1);

    while (nextGen.size() < N) {
        auto clone = std::make_unique<FeedForwardGenome>(*nextGen[pick(m_Rng)]);
        clone->Mutate(0.1f, 0.3f);
        nextGen.push_back(std::move(clone));
    }

    m_Genomes = std::move(nextGen);
    std::fill(m_Fitness.begin(), m_Fitness.end(), 0.0f);
}

}  // namespace gr
