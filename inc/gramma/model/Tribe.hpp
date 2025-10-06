#pragma once
#include <glm/vec3.hpp>
#include <gramma/model/Home.hpp>

namespace gr {

struct TribePolicy {
    bool shareInside{true};
    bool allowRaid{false};
    float raidAggressiveness{0.0f};  // 0..1
};

class Tribe : public Home {
   public:
    Tribe(int id, const glm::vec2& pos, int maxOcc, glm::vec3 color, TribePolicy pol = {})
        : Home(pos, maxOcc), m_Id(id), m_Color(color), m_Policy(pol) {
    }

    int Id() const {
        return m_Id;
    }
    glm::vec3 Color() const {
        return m_Color;
    }
    const TribePolicy& Policy() const {
        return m_Policy;
    }

   private:
    int m_Id{0};
    glm::vec3 m_Color{1, 1, 1};
    TribePolicy m_Policy{};
};

}  // namespace gr
