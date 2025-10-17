#include <gramma/model/evolution/FeedForwardGenome.hpp>
#include <iostream>
#include <random>

using namespace std;

namespace gr {

// ============================================================================
// Constructor
// ============================================================================
FeedForwardGenome::FeedForwardGenome(std::vector<int> layers, ActivationType act)
    : m_Layers(std::move(layers)), m_ActType(act) {
    if (m_Layers.size() < 2) {
        std::cerr << "FeedForwardGenome requires at least input and output layer." << std::endl;
        throw std::runtime_error("Invalid layer configuration");
    }

    m_Weights.reserve(m_Layers.size() - 1);
    m_Biases.reserve(m_Layers.size() - 1);

    for (size_t i = 0; i < m_Layers.size() - 1; ++i) {
        m_Weights.emplace_back(m_Layers[i + 1], m_Layers[i]);
        m_Biases.emplace_back(m_Layers[i + 1]);
    }
    Randomize();
}

// ============================================================================
// Activation
// ============================================================================
Eigen::VectorXf FeedForwardGenome::Activate(const Eigen::VectorXf& x, ActivationType type) {
    switch (type) {
        case ActivationType::ReLU: {
            return x.cwiseMax(0.0f);
        }
        case ActivationType::Sigmoid: {
            return (1.0f / (1.0f + (-x.array()).exp())).matrix();
        }
        case ActivationType::Tanh: {
            return x.array().tanh().matrix();
        }
        case ActivationType::Linear:
        default:
            return x;
    }
}

// ============================================================================
// Forward
// ============================================================================
std::vector<float> FeedForwardGenome::Forward(const std::vector<float>& input) const {
    Eigen::VectorXf x = Eigen::Map<const Eigen::VectorXf>(input.data(), input.size());

    for (size_t i = 0; i < m_Weights.size(); ++i) {
        x = m_Weights[i] * x + m_Biases[i];
        if (i < m_Weights.size() - 1) {
            x = Activate(x, m_ActType);
        }
    }

    return std::vector<float>(x.data(), x.data() + x.size());
}

// ============================================================================
// Randomize
// ============================================================================
void FeedForwardGenome::Randomize() {
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<float> dist(0.0f, 0.5f);

    for (auto& W : m_Weights) W = Mat::NullaryExpr(W.rows(), W.cols(), [&]() { return dist(rng); });

    for (auto& b : m_Biases) b = Vec::NullaryExpr(b.size(), [&]() { return dist(rng); });
}

// ============================================================================
// Mutate
// ============================================================================
void FeedForwardGenome::Mutate(float rate, float strength) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> prob(0.0f, 1.0f);
    std::normal_distribution<float> delta(0.0f, strength);

    for (auto& W : m_Weights) {
        for (int r = 0; r < W.rows(); ++r) {
            for (int c = 0; c < W.cols(); ++c) {
                if (prob(rng) < rate) {
                    W(r, c) += delta(rng);
                }
            }
        }
    }

    for (auto& b : m_Biases) {
        for (int i = 0; i < b.size(); ++i) {
            if (prob(rng) < rate) {
                b(i) += delta(rng);
            }
        }
    }
}

// ============================================================================
// Parameter count
// ============================================================================
size_t FeedForwardGenome::ParameterCount() const {
    size_t total = 0;
    for (size_t i = 0; i < m_Weights.size(); ++i) total += m_Weights[i].size() + m_Biases[i].size();
    return total;
}

}  // namespace gr
