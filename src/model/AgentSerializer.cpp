#include <fstream>
#include <gramma/model/AgentSerializer.hpp>
#include <iostream>
#include <stdexcept>

namespace gr {

using json = nlohmann::json;

json AgentSerializer::SerializeTraits(const AgentTraits& traits) const {
    json j;
    j["age"] = static_cast<int>(traits.age);
    j["sex"] = static_cast<int>(traits.sex);
    j["mass"] = traits.mass;
    j["bodyRadius"] = traits.bodyRadius;
    j["socialRadius"] = traits.socialRadius;
    j["maxSpeed"] = traits.maxSpeed;
    return j;
}

std::unique_ptr<AgentTraits> AgentSerializer::DeserializeTraits(const json& j) const {
    auto traits = std::make_unique<AgentTraits>();
    if (j.contains("age")) traits->age = static_cast<AgeClass>(j["age"].get<int>());
    if (j.contains("sex")) traits->sex = static_cast<Sex>(j["sex"].get<int>());
    if (j.contains("mass")) traits->mass = j["mass"].get<float>();
    if (j.contains("bodyRadius")) traits->bodyRadius = j["bodyRadius"].get<float>();
    if (j.contains("socialRadius")) traits->socialRadius = j["socialRadius"].get<float>();
    if (j.contains("maxSpeed")) traits->maxSpeed = j["maxSpeed"].get<float>();
    return traits;
}

json AgentSerializer::SerializeAgent(const Agent& agent) const {
    json j;
    j["id"] = agent.GetId();
    j["position"] = {agent.GetPosition().x, agent.GetPosition().y};
    j["heading"] = {agent.GetHeading().x, agent.GetHeading().y};

    // Traits
    j["traits"] = SerializeTraits(agent.GetTraits());

    // Needs – optional
    j["needs"] = json::array();
    for (const auto& need : agent.GetNeeds()) {
        j["needs"].push_back(need->Name());
    }

    return j;
}

void AgentSerializer::DeserializeAgent(const json& j, Agent& agent) const {
    if (j.contains("id")) agent.SetId(j["id"].get<std::string>());

    if (j.contains("position")) {
        auto pos = j["position"];
        agent.SetPosition({pos[0].get<float>(), pos[1].get<float>()});
    }

    if (j.contains("heading")) {
        auto head = j["heading"];
        agent.SetHeading({head[0].get<float>(), head[1].get<float>()});
    }

    if (j.contains("traits")) {
        agent.SetTraits(DeserializeTraits(j["traits"]));
    }

    // Needs Optional
    if (j.contains("needs")) {
    }
}

bool AgentSerializer::Save(const Agent& agent, const std::string& filePath) const {
    try {
        json j = SerializeAgent(agent);
        std::ofstream file(filePath);
        if (!file.is_open()) return false;
        file << std::setw(4) << j;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[AgentSerializer] Save failed: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<Agent> AgentSerializer::Load(const std::string& filePath) const {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) throw std::runtime_error("File not found");

        json j;
        file >> j;

        auto agent = std::make_unique<Agent>();
        DeserializeAgent(j, *agent);
        return agent;
    } catch (const std::exception& e) {
        std::cerr << "[AgentSerializer] Load failed: " << e.what() << std::endl;
        return nullptr;
    }
}

}  // namespace gr
