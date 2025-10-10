#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/Agent.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace gr {

/**
 * @brief Provides functionality to load and save agents to/from JSON files.
 */
class AgentSerializer {
   public:
    AgentSerializer() = default;
    ~AgentSerializer() = default;

    /**
     * @brief Saves a single agent to a JSON file.
     * @param agent The agent to serialize.
     * @param filePath Output file path.
     * @return true if successful.
     */
    bool Save(const Agent& agent, const std::string& filePath) const;

    /**
     * @brief Loads a single agent from a JSON file.
     * @param filePath Path to JSON file.
     * @return A new Agent instance or nullptr if loading failed.
     */
    std::unique_ptr<Agent> Load(const std::string& filePath) const;

   private:
    nlohmann::json SerializeAgent(const Agent& agent) const;
    nlohmann::json SerializeTraits(const AgentTraits& traits) const;

    void DeserializeAgent(const nlohmann::json& j, Agent& agent) const;
    std::unique_ptr<AgentTraits> DeserializeTraits(const nlohmann::json& j) const;
};

}  // namespace gr
