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
    glm::vec2 mouse = glm::vec2(data->mouse.x, data->mouse.y);

    for (auto &[entity, cBone] : m_Registry->Get<EBone, CBone>()) {
      CBone::Part part = HoverSystem::HoveredOver(cBone, mouse);

      if (entity->Has<CSelected>() && entity->Get<CSelected>()->target == part)
        continue;

      if (part == CBone::Part::None) {
        if (entity->Has<CHovered>()) {
          entity->Remove<CHovered>();

          Animate::Once<glm::vec4, ECS::Entity>::Create()
              ->Duration(0.25f)
              ->Value(&cBone->color, Colors::DEFAULT)
              ->Value(&cBone->joints[0].color, Colors::DEFAULT)
              ->Value(&cBone->joints[1].color, Colors::DEFAULT)
              ->OnUpdate(
                  entity,
                  [](ECS::Entity *entity) { entity->MarkChanged<CBone>(); })
              ->Play();
        }
        continue;
      }

      CHovered *component = entity->Ensure<CHovered>();

      bool mutated = component->target != part;

      component->target = part;

      if (mutated) {
        entity->MarkChanged<CBone>();

        auto animation = Animate::Once<glm::vec4, ECS::Entity>::Create()
                             ->Duration(0.25f)
                             ->OnUpdate(entity, [](ECS::Entity *entity) {
                               entity->MarkChanged<CBone>();
                             });

        if (part == CBone::StartJoint) {
          animation->Value(&cBone->joints[CBone::StartJoint].color,
                           Colors::HIGHLIGHT);
          animation->Value(&cBone->joints[CBone::EndJoint].color,
                           Colors::DEFAULT);
          animation->Value(&cBone->color, Colors::DEFAULT);
        }

        if (part == CBone::EndJoint) {
          animation->Value(&cBone->joints[CBone::EndJoint].color,
                           Colors::HIGHLIGHT);
          animation->Value(&cBone->joints[CBone::StartJoint].color,
                           Colors::DEFAULT);
          animation->Value(&cBone->color, Colors::DEFAULT);
        }

        if (part == CBone::Shaft)
          animation->Value(&cBone->color, Colors::HIGHLIGHT)
              ->Value(&cBone->joints[0].color, Colors::HIGHLIGHT)
              ->Value(&cBone->joints[1].color, Colors::HIGHLIGHT);

        animation->Play();
      }

      entity->ClearChanged<CHovered>();
    }
  }
};