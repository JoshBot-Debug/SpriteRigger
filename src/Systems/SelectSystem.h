#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS2/Registry.h"
#include "ECS2/System.h"

#include "Camera/OrthographicCamera.h"
#include "Common.h"

class SelectSystem : public ECS2::System {
private:
  Grid *m_Grid = nullptr;
  ECS2::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

public:
  void Free() {
    m_Grid = nullptr;
    m_Camera = nullptr;
    m_Registry = nullptr;
  }

  void Initialize(ECS2::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Grid = grid;
    m_Camera = camera;
    m_Registry = registry;
  };

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    if (!data->isMouseClicked)
      return;

    for (auto [entity, cSelected] : m_Registry->Get<EBone, CSelected>()) {
      if (!entity->MarkedForRemoval<CSelected>()) {
        std::cout << "MarkForRemoval CSelected " << (int)cSelected->target
                  << std::endl;
        auto cHovered = entity->Get<CHovered>();
        if (cHovered && cHovered->target != cSelected->target) {
          ECS2::Mutate<CSelected, CBone::Part>(entity, cSelected->target,
                                               cHovered->target);
          continue;
        }
        cSelected->target = CBone::None;
        entity->MarkForRemoval<CSelected>();
      }
    }

    for (auto [entity, cHovered] : m_Registry->Get<EBone, CHovered>()) {
      if (!entity->Has<CSelected>()) {
        std::cout << "Add CSelected " << (int)cHovered->target << std::endl;
        entity->Add<CSelected>(cHovered->target);
      }
    }
  }
};