#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Registry.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class DragSystem : public ECS::System {
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
    auto mouse = glm::vec2(data->mouse.x, data->mouse.y);
    auto deltaMouse = glm::vec2(data->deltaMouse.x, data->deltaMouse.y);

    for (auto [entity, cDragging] : m_Registry->Get<EBone, CDragging>()) {
      auto cBone = entity->Get<CBone>();
      
      if (cDragging->target == CBone::Shaft) {
        auto &sp = cBone->joints[CBone::StartJoint].position;
        auto &ep = cBone->joints[CBone::EndJoint].position;
        ECS::Mutate<CBone, glm::vec2>(entity, sp, sp + deltaMouse);
        ECS::Mutate<CBone, glm::vec2>(entity, ep, ep + deltaMouse);
      } else {
        auto &position = cBone->joints[cDragging->target].position;
        ECS::Mutate<CBone, glm::vec2>(entity, position, mouse);
      }
    }
  }
};