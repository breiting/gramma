#include <chrono>
#include <glm/glm.hpp>
#include <gramma/model/agent/Agent.hpp>
#include <gramma/model/agent/AgentFactory.hpp>
#include <gramma/model/environment/Environment.hpp>

namespace gr {

AgentFactory::AgentFactory() {
    m_Rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
}

}  // namespace gr
