#pragma once
#include <gramma/model/evolution/Genome.hpp>
#include <vector>

namespace gr {

class Population {
   public:
    Population(int size, int in, int hidden, int out);
    void Evolve();

    std::vector<Genome>& GetGenomes() {
        return m_Genomes;
    }

    Genome GetBestGenome() const {
        return m_Genomes[0];
    }

   private:
    int m_In, m_Hid, m_Out;
    std::vector<Genome> m_Genomes;
};

}  // namespace gr
