#include "gramma/model/evolution/Genome.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>

using json = nlohmann::json;

using namespace std;

namespace gr {

static float sigmoid(float x) {
    return 1.f / (1.f + expf(-x));
}

Genome::Genome(int in, int hidden, int out) : m_In(in), m_Hid(hidden), m_Out(out) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.f, 1.f);
    m_W1.resize(in * hidden);
    m_W2.resize(hidden * out);
    generate(m_W1.begin(), m_W1.end(), [&] { return dist(rng); });
    generate(m_W2.begin(), m_W2.end(), [&] { return dist(rng); });
}

vector<float> Genome::Forward(const vector<float>& input) const {
    vector<float> hidden(m_Hid);
    for (int h = 0; h < m_Hid; ++h) {
        float sum = 0;
        for (int i = 0; i < m_In; ++i) sum += input[i] * m_W1[h * m_In + i];
        hidden[h] = sigmoid(sum);
    }
    vector<float> out(m_Out);
    for (int o = 0; o < m_Out; ++o) {
        float sum = 0;
        for (int h = 0; h < m_Hid; ++h) sum += hidden[h] * m_W2[o * m_Hid + h];
        out[o] = tanhf(sum);
    }
    return out;
}

void Genome::Mutate(float rate) {
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<float> noise(0.f, 0.3f);
    for (auto& w : m_W1)
        if (((float)rand() / RAND_MAX) < rate) w += noise(rng);
    for (auto& w : m_W2)
        if (((float)rand() / RAND_MAX) < rate) w += noise(rng);
}

Genome Genome::Crossover(const Genome& a, const Genome& b) {
    Genome child(a.m_In, a.m_Hid, a.m_Out);
    for (size_t i = 0; i < child.m_W1.size(); ++i) child.m_W1[i] = (rand() % 2) ? a.m_W1[i] : b.m_W1[i];
    for (size_t i = 0; i < child.m_W2.size(); ++i) child.m_W2[i] = (rand() % 2) ? a.m_W2[i] : b.m_W2[i];
    return child;
}

void Genome::SetFitness(float fitness) {
    m_Fitness = fitness;
}

float Genome::GetFitness() const {
    return m_Fitness;
}

void Genome::Save(const std::string& fileName) const {
    json j;
    j["in"] = m_In;
    j["hidden"] = m_Hid;
    j["out"] = m_Out;
    j["w1"] = m_W1;
    j["w2"] = m_W2;
    std::ofstream file(fileName);
    if (!file.is_open()) return;
    file << std::setw(4) << j;
}

Genome Genome::Load(const std::string& fileName) const {
    std::ifstream file(fileName);
    if (!file.is_open()) throw std::runtime_error("File not found");

    json j;
    file >> j;
    auto in = j["in"];
    auto hidden = j["hidden"];
    auto out = j["out"];
    Genome g(in, hidden, out);
    return g;
    // m_W1 = j["w1"].get<std::vector<float>>();
    // m_W2 = j["w2"].get<std::vector<float>>();
}

}  // namespace gr
