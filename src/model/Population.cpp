#include "gramma/model/evolution/Population.hpp"

#include <algorithm>
#include <iostream>

using namespace std;
namespace gr {

Population::Population(int size, int in, int hidden, int out) : m_In(in), m_Hid(hidden), m_Out(out) {
    for (int i = 0; i < size; ++i) {
        m_Genomes.emplace_back(in, hidden, out);
    }
}

void Population::Evolve() {
    std::sort(m_Genomes.begin(), m_Genomes.end(), [](auto& a, auto& b) { return a.GetFitness() > b.GetFitness(); });

    int eliteCount = m_Genomes.size() / 10;

    cout << "Best Fitness: " << m_Genomes[0].GetFitness() << endl;

    std::vector<Genome> next;
    next.insert(next.end(), m_Genomes.begin(), m_Genomes.begin() + eliteCount);

    while (next.size() < m_Genomes.size()) {
        const auto& a = m_Genomes[rand() % eliteCount];
        const auto& b = m_Genomes[rand() % eliteCount];
        auto child = Genome::Crossover(a, b);
        child.Mutate();
        next.push_back(std::move(child));
    }

    m_Genomes.swap(next);
}

}  // namespace gr
