#include <gramma/model/task/TaskFactory.hpp>
#include <iostream>

namespace gr {

TaskFactory& TaskFactory::Instance() {
    static TaskFactory instance;
    return instance;
}

void TaskFactory::RegisterBuilder(const std::string& needName, std::shared_ptr<INeedTaskBuilder> builder) {
    m_Builders[needName] = std::move(builder);
}

std::unique_ptr<ITask> TaskFactory::BuildTaskForNeed(const INeed& need, Agent& agent, const Environment& env) const {
    auto it = m_Builders.find(need.Name());
    if (it != m_Builders.end()) {
        return it->second->Build(need, agent, env);
    }

    std::cerr << "[TaskFactory] No builder found for need: " << need.Name() << std::endl;
    return nullptr;
}

}  // namespace gr
