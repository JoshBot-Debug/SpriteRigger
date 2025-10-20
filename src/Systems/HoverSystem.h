#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS2/Registry.h"
#include "ECS2/System.h"
#include "ECS2/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class HoverSystem : public ECS2::System {
private:
  Grid *m_Grid = nullptr;
  ECS2::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

private:
  CBone::Part HoveredOver(CBone *bone, glm::vec2 mouse) {
    auto &start = bone->joints[CBone::StartJoint];
    auto &end = bone->joints[CBone::EndJoint];

    float radius = bone->thickness * 0.5f;
    glm::vec2 direction = glm::normalize(start.position - end.position);
    glm::vec2 offset = direction * radius * radius;

    if (Intersect::Circle(mouse, start.position - offset, radius))
      return CBone::Part::StartJoint;
    if (Intersect::Circle(mouse, end.position + offset, radius))
      return CBone::Part::EndJoint;
    if (Intersect::Line(mouse, start.position - offset, end.position + offset,
                        bone->thickness))
      return CBone::Part::Shaft;

    return CBone::Part::None;
  }

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
    glm::vec2 mouse = glm::vec2(data->mouse.x, data->mouse.y);

    for (auto [entity, cBone] : m_Registry->Get<EBone, CBone>()) {
      auto cHovered = entity->Get<CHovered>();

      if (entity->MarkedForRemoval<CHovered>())
        continue;

      CBone::Part part = HoverSystem::HoveredOver(cBone, mouse);

      if (cHovered && part == CBone::Part::None) {
        entity->MarkForRemoval<CHovered>();
        cHovered->target = CBone::Part::None;
        continue;
      }

      if (part != CBone::Part::None)
        if (!cHovered)
          cHovered = entity->Add<CHovered>(part);
        else
          ECS2::Mutate<CHovered, CBone::Part>(entity, cHovered->target, part);
    }
  }
};