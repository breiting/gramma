#pragma once

#include <Eigen/Dense>
#include <vector>

namespace gr {

/**
 * @brief Enum for selectable activation functions.
 */
enum class ActivationType {
    ReLU,     ///< Rectified Linear Unit
    Sigmoid,  ///< Logistic sigmoid
    Tanh,     ///< Hyperbolic tangent
    Linear    ///< Identity function
};

/**
 * @class FeedForwardGenome
 * @brief Fixed-topology, fully-connected neural network genome used for neuroevolution.
 *
 * Implements an efficient matrix-based forward pass using Eigen 5.
 * The network structure is defined at construction time (e.g. {2, 8, 2})
 * and remains fixed during evolution. Only weights and biases are mutated.
 *
 * Typical usage:
 * @code
 * gr::FeedForwardGenome nn({2, 8, 2}, gr::ActivationType::Tanh);
 * nn.Randomize();
 * auto output = nn.Forward({0.5f, -0.2f});
 * @endcode
 */
class FeedForwardGenome {
   public:
    using Mat = Eigen::MatrixXf;
    using Vec = Eigen::VectorXf;

    /**
     * @brief Construct a feed-forward network.
     * @param layers Layer configuration (e.g. {2, 8, 2})
     * @param act Activation function type
     */
    explicit FeedForwardGenome(std::vector<int> layers, ActivationType act = ActivationType::Tanh);

    /**
     * @brief Execute a forward pass through the network.
     * @param input Input vector (must match first layer size)
     * @return Output vector (size of last layer)
     */
    std::vector<float> Forward(const std::vector<float>& input) const;

    /**
     * @brief Randomize all weights and biases (Gaussian distribution).
     */
    void Randomize();

    /**
     * @brief Apply small random mutations to weights and biases.
     * @param rate Probability [0..1] that a given parameter mutates
     * @param strength Standard deviation of mutation noise
     */
    void Mutate(float rate = 0.1f, float strength = 0.5f);

    /**
     * @return Total number of parameters in this genome.
     */
    size_t ParameterCount() const;

    /** @return Access to layer structure. */
    const std::vector<int>& GetLayers() const {
        return m_Layers;
    }

    /** @return Activation type used by this network. */
    ActivationType GetActivationType() const {
        return m_ActType;
    }

   private:
    static Eigen::VectorXf Activate(const Eigen::VectorXf& x, ActivationType type);

   private:
    std::vector<Mat> m_Weights;  ///< Layer weight matrices
    std::vector<Vec> m_Biases;   ///< Layer bias vectors
    std::vector<int> m_Layers;   ///< Layer size configuration
    ActivationType m_ActType;    ///< Activation function
};

}  // namespace gr
