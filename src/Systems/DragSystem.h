#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS2/Registry.h"
#include "ECS2/System.h"
#include "ECS2/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class DragSystem : public ECS2::System {
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
    auto mouse = glm::vec2(data->mouse.x, data->mouse.y);
    auto deltaMouse = glm::vec2(data->deltaMouse.x, data->deltaMouse.y);

    for (auto [entity, cSelected] : m_Registry->Get<EBone, CSelected>()) {
      auto cBone = entity->Get<CBone>();
      
      if (cSelected->target == CBone::Shaft) {
        auto &sp = cBone->joints[CBone::StartJoint].position;
        auto &ep = cBone->joints[CBone::EndJoint].position;
        ECS2::Mutate<CBone, glm::vec2>(entity, sp, sp + deltaMouse);
        ECS2::Mutate<CBone, glm::vec2>(entity, ep, ep + deltaMouse);
      } else {
        auto &position = cBone->joints[cSelected->target].position;
        ECS2::Mutate<CBone, glm::vec2>(entity, position, mouse);
      }
    }
  }
};