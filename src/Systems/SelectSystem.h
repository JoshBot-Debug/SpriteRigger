#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"

#include "Camera/OrthographicCamera.h"
#include "Common.h"

class SelectSystem : public ECS::System {
private:
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

public:
  void Free() {
    m_Grid = nullptr;
    m_Camera = nullptr;
    m_Registry = nullptr;
  }

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Grid = grid;
    m_Camera = camera;
    m_Registry = registry;
  };

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    if (!data->isMouseClicked)
      return;

    for (auto [eid, cSelected] : m_Registry->Get<CSelected>()) {
      if (!m_Registry->MarkedForRemoval<CSelected>(eid)) {
        std::cout << "MarkForRemoval CSelected " << (int)cSelected->target
                  << std::endl;
        auto cHovered = m_Registry->Get<CHovered>(eid);
        if (cHovered && cHovered->target != cSelected->target) {
          ECS::Mutate<CSelected, CBone::Part>(m_Registry, eid, cSelected->target, cHovered->target);
          continue;
        }
        cSelected->target = CBone::None;
        m_Registry->MarkForRemoval<CSelected>(eid);
      }
    }

    for (auto [eid, cHovered] : m_Registry->Get<CHovered>()) {
      if (!m_Registry->Has<CSelected>(eid)) {
        std::cout << "Add CSelected " << (int)cHovered->target << std::endl;
        m_Registry->Add<CSelected>(eid, cHovered->target);
      }
    }
  }
};