#pragma once

#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

#include "Animate/Once.h"
#include "Animate/Timeline.h"

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
    if (data->isMouseClicked) {
      std::cout << "Remove all" << std::endl;
      m_Registry->Remove<EBone, CSelected>();
      for (auto &[entity, cHovered] : m_Registry->Get<EBone, CHovered>()) {
        entity->Add<CSelected>(cHovered->target);
        entity->Remove<CHovered>();
      }
    }
  }
};