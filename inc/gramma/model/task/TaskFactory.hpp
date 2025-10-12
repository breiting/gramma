#pragma once

#include <gramma/model/task/INeedTaskBuilder.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace gr {

/**
 * Central factory for creating tasks based on current agent needs.
 * Supports dynamic registration of need-to-task mappings.
 */
class TaskFactory {
   public:
    static TaskFactory& Instance();

    void RegisterBuilder(const std::string& needName, std::shared_ptr<INeedTaskBuilder> builder);

    std::unique_ptr<ITask> BuildTaskForNeed(const INeed& need, Agent& agent, const Environment& env) const;

   private:
    std::unordered_map<std::string, std::shared_ptr<INeedTaskBuilder>> m_Builders;
};

}  // namespace gr
