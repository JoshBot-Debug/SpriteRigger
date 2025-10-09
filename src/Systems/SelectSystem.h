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

    for (auto [eid, cBone] : m_Registry->Get<CBone>()) {
      if (!m_Registry->Has<CHovered>(eid)) {
        if (data->isMouseDown && !m_Registry->MarkedForRemoval<CSelected>(eid))
          m_Registry->MarkForRemoval<CSelected>(eid);

        continue;
      }

      for (auto [id, cHovered] : m_Registry->Get<CHovered>()) {
        if (data->isMouseDown)
          m_Registry->Add<CSelected>(id, cHovered->target);
      }
    }

    // if (!m_Registry->Has<CHovered>()) {
    //   if (data->isMouseDown)
    //     m_Registry->Remove<CSelected>();
    //   return;
    // }

    // for (auto [id, cHovered] : m_Registry->Get<CHovered>()) {
    //   if (data->isMouseDown)
    //     m_Registry->Add<CSelected>(id, cHovered->target);
    // }
  }
};