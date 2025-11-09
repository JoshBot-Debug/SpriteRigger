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

class HoverSystem : public ECS::System {
private:
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
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

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Grid = grid;
    m_Camera = camera;
    m_Registry = registry;
  };

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);
    glm::vec2 mouse = {data->mouse.x, data->mouse.y};

    for (auto &[entity, cBone] : m_Registry->Get<EBone, CBone>()) {

      CBone::Part hovered = HoverSystem::HoveredOver(cBone, mouse);

      CHovered *cHovered = nullptr;
      CSelected *cSelected = entity->Get<CSelected>();

      if (hovered != CBone::Part::None) {
        cHovered = entity->Ensure<CHovered>();
        cHovered->target = hovered;
      } else if (entity->Has<CHovered>())
        entity->Remove<CHovered>();

      auto resolve = [cHovered, cSelected](CBone::Part part) {
        if (cHovered && cHovered->target == CBone::Shaft)
          return Colors::HIGHLIGHT;
        if (cSelected && cSelected->target == CBone::Shaft)
          return Colors::HIGHLIGHT;
        if (cSelected && cSelected->target == part)
          return Colors::HIGHLIGHT;
        if (cHovered && cHovered->target == part)
          return Colors::HIGHLIGHT;
        return Colors::DEFAULT;
      };

      glm::vec4 targetShaft = resolve(CBone::Shaft);
      glm::vec4 targetStartJoint = resolve(CBone::StartJoint);
      glm::vec4 targetEndJoint = resolve(CBone::EndJoint);

      Animate::Once<glm::vec4, ECS::Entity>::Create()
          ->Duration(ANIMATION_DURATION)
          ->Value(&cBone->color, targetShaft)
          ->Value(&cBone->joints[0].color, targetStartJoint)
          ->Value(&cBone->joints[1].color, targetEndJoint)
          ->OnUpdate(entity, [](ECS::Entity *e) { e->MarkChanged<CBone>(); })
          ->Play();
    }
  }
};