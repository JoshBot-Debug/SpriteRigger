#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class HoverSystem : public ECS::System {
private:
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

public:
  ~HoverSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Registry = registry;
    m_Grid = grid;
    m_Camera = camera;
  };

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);
    glm::vec2 mouse = glm::vec2(data->mouse.x, data->mouse.y);

    for (auto [eid, cBone] : m_Registry->Get<CBone>()) {
      auto entity = m_Registry->GetEntity(eid);
      auto cHovered = m_Registry->Get<CHovered>(eid);

      CBone::Part part = HoverSystem::HoveredOver(cBone, mouse);

      if (!cHovered)
        cHovered = entity->Add<CHovered>(part);
      else if (part == CBone::Part::None)
        cHovered = entity->Remove<CHovered>();
      else
        ECS::Mutate<CHovered, CBone::Part>(m_Registry, cHovered->target, part);
    }
  }

  static CBone::Part HoveredOver(CBone *bone, glm::vec2 mouse) {
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
};