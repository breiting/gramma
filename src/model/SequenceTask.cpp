#include <gramma/model/task/SequenceTask.hpp>

namespace gr {

SequenceTask::SequenceTask(std::vector<std::unique_ptr<ITask>> tasks) : m_Tasks(std::move(tasks)) {
}

SequenceTask::SequenceTask(std::unique_ptr<ITask> first, std::unique_ptr<ITask> second) {
    if (first) m_Tasks.push_back(std::move(first));
    if (second) m_Tasks.push_back(std::move(second));
}

void SequenceTask::Start(Agent& agent) {
    if (m_Tasks.empty()) return;
    m_Tasks[m_CurrentIndex]->Start(agent);
    m_Started = true;
}

void SequenceTask::Update(Agent& agent, float dt) {
    if (!m_Started || m_Tasks.empty()) return;

    auto& current = m_Tasks[m_CurrentIndex];
    current->Update(agent, dt);

    if (current->IsFinished()) {
        m_CurrentIndex++;
        if (m_CurrentIndex < m_Tasks.size()) {
            m_Tasks[m_CurrentIndex]->Start(agent);
        }
    }
}

bool SequenceTask::IsFinished() const {
    return m_CurrentIndex >= m_Tasks.size();
}

void SequenceTask::Add(std::unique_ptr<ITask> task) {
    m_Tasks.push_back(std::move(task));
}

}  // namespace gr
