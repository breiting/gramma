#pragma once
#include <glm/vec2.hpp>
#include <gramma/model/IResource.hpp>
#include <string>

namespace gr {

class Exit : public IResource {
   public:
    Exit(const glm::vec2& pos, std::string label = "", float width = 1.0f)
        : m_Pos(pos), m_Width(width), m_Label(std::move(label)) {
    }

    ResourceType GetType() const override {
        return ResourceType::Exit;
    }

    const glm::vec2& GetPosition() const override {
        return m_Pos;
    }

    float Consume(float) override {
        return 1.0f;
    }

    void Regenerate(float) override {
    }

    bool IsDepleted() const override {
        return false;
    }

    bool IsBlocked() const {
        return m_IsBlocked;
    }
    float GetWidth() const {
        return m_Width;
    }
    const std::string& GetLabel() const {
        return m_Label;
    }

   private:
    glm::vec2 m_Pos{0.0f, 0.0f};
    bool m_IsBlocked{false};
    float m_Width{1.0f};  // in Metern
    std::string m_Label;
};

}  // namespace gr
