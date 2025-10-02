#include <cstdlib>
#include <glm/glm.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/RelocateTask.hpp>

namespace gr {

RelocateTask::RelocateTask(int width, int height) : m_Width(width), m_Height(height) {
}

void RelocateTask::Start(Agent& agent) {
    // Random position within bounds
    float x = -m_Width / 2.0 + static_cast<float>(rand()) / RAND_MAX * m_Width;
    float y = -m_Height / 2.0 + static_cast<float>(rand()) / RAND_MAX * m_Height;

    agent.SetPosition({x, y});
    m_Done = true;
}

void RelocateTask::Update(Agent& /*agent*/, float /*dt*/) {
    // do nothing
}

}  // namespace gr
