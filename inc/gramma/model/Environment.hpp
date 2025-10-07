#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <gramma/model/Agent.hpp>
#include <gramma/model/Home.hpp>
#include <gramma/model/ICollisionHandler.hpp>
#include <gramma/model/IResource.hpp>
#include <gramma/model/Types.hpp>
#include <memory>
#include <nanoflann.hpp>
#include <vector>

namespace gr {

/**
 * Wrapper for KD-tree of Agents
 */
struct AgentCloud {
    const std::vector<std::unique_ptr<Agent>>* agents = nullptr;

    inline size_t kdtree_get_point_count() const {
        return agents ? agents->size() : 0;
    }

    inline double kdtree_get_pt(const size_t idx, int dim) const {
        const auto& pos = (*agents)[idx]->GetPosition();
        return (dim == 0) ? pos.x : pos.y;
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const {
        return false;
    }
};

/**
 * Global environment that contains agents and food sources.
 * Responsible for updating the simulation state and rendering.
 */

class Environment {
   public:
    Environment(float xmin, float xmax, float ymin, float ymax);

    glm::vec2 RandomPosition() const;  // optional: implementieren im .cpp

    void AddAgent(std::unique_ptr<Agent> a);
    void AddResource(std::shared_ptr<IResource> r);
    void AddHome(std::shared_ptr<Home> h);

    const std::vector<std::unique_ptr<Agent>>& Agents() const {
        return m_Agents;
    }
    const std::vector<std::shared_ptr<IResource>>& Resources() const {
        return m_Resources;
    }
    const std::vector<std::shared_ptr<Home>>& Homes() const {
        return m_Homes;
    }
    Home* GetNextFreeHome();

    // Update-Loop
    void Update(float dt);
    void Render(const glm::mat4& vp);

    void BuildSpatialIndex();
    using KDTreeType =
        nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<double, AgentCloud>, AgentCloud, 2, size_t>;

    // TODO: remove this
    KDTreeType& GetKDTree() const {
        return *m_KDTree;
    }
    const AgentCloud& GetAgentCloud() const {
        return m_Cloud;
    }

    void SetCollisionHandler(std::unique_ptr<ICollisionHandler> h) {
        m_CollisionHandler = std::move(h);
    }

    void Stats() const;

    float XMin() const {
        return m_Xmin;
    }
    float XMax() const {
        return m_Xmax;
    }
    float YMin() const {
        return m_Ymin;
    }
    float YMax() const {
        return m_Ymax;
    }

    float GetWidth() const {
        return m_Xmax - m_Xmin;
    }

    float GetHeight() const {
        return m_Ymax - m_Ymin;
    }

    const Agent* GetAgent(size_t idx) const {
        return m_Agents[idx].get();
    }

    // Utility: nächste Ressource vom Typ
    std::shared_ptr<IResource> FindNearest(ResourceType type, const glm::vec2& pos) const;

   private:
    float m_Xmin, m_Xmax, m_Ymin, m_Ymax;
    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::vector<std::shared_ptr<IResource>> m_Resources;
    std::vector<std::shared_ptr<Home>> m_Homes;
    std::unique_ptr<ICollisionHandler> m_CollisionHandler;
    AgentCloud m_Cloud;
    std::unique_ptr<KDTreeType> m_KDTree;
};

}  // namespace gr
