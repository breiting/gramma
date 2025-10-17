#pragma once

#include <tbb/parallel_for.h>

#include <gramma/model/evolution/FeedForwardGenome.hpp>
#include <memory>
#include <random>
#include <vector>

namespace gr {

/**
 * @class Population
 * @brief Manages a set of neural network genomes, providing evolution operations.
 */
class Population {
   public:
    explicit Population(size_t size, std::vector<int> topology, ActivationType act = ActivationType::Tanh);

    /**
     * @brief Get reference to all genomes.
     */
    std::vector<std::unique_ptr<FeedForwardGenome>>& GetGenomes() {
        return m_Genomes;
    }

    /**
     * @brief Assign fitness values to each genome.
     * @param fitness Array of fitness values corresponding to each genome.
     */
    void SetFitness(const std::vector<float>& fitness);

    /**
     * @brief Perform selection, crossover (optional), and mutation.
     */
    void Evolve();

    /**
     * @brief Return best genome of current generation.
     */
    const FeedForwardGenome& GetBestGenome() const;

   private:
    std::vector<std::unique_ptr<FeedForwardGenome>> m_Genomes;
    std::vector<float> m_Fitness;

    std::vector<int> m_Topology;
    ActivationType m_ActType;

    mutable std::mt19937 m_Rng;
};

}  // namespace gr
