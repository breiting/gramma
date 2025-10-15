#pragma once
#include <vector>

namespace gr {

class Genome {
   public:
    Genome(int in, int hidden, int out);
    std::vector<float> Forward(const std::vector<float>& input) const;

    void Mutate(float rate = 0.1f);
    static Genome Crossover(const Genome& a, const Genome& b);

    void SetFitness(float fitness);
    float GetFitness() const;

    void Save(const std::string& fileName) const;
    Genome Load(const std::string& fileName) const;

   private:
    std::vector<float> m_W1;  // input->hidden
    std::vector<float> m_W2;  // hidden->output
    int m_In, m_Hid, m_Out;
    float m_Fitness = 0.0f;
};

}  // namespace gr
