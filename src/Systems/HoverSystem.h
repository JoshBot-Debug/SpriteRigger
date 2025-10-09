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

  CBone::Part HoveredOver(CBone *bone, glm::vec2 mouse) {
    auto &start = bone->joints[CBone::StartJoint];
    auto &end = bone->joints[CBone::EndJoint];

    if (Intersect::Circle(mouse, start.position, bone->thickness))
      return CBone::Part::StartJoint;
    if (Intersect::Circle(mouse, end.position, bone->thickness))
      return CBone::Part::EndJoint;
    if (Intersect::Line(mouse, start.position, end.position, bone->thickness))
      return CBone::Part::Shaft;

    return CBone::Part::None;
  }

  // May want to create a ColorInterpolatingSystem to interpolate colors
  void UpdateColor(ECS::Registry *registry, CBone *bone, CHovered *hovered,
                   float deltaTime) {
    auto &c = bone->color;
    auto &s = bone->joints[CBone::StartJoint].color;
    auto &e = bone->joints[CBone::EndJoint].color;

    auto ch = Colors::DEFAULT;
    auto sh = Colors::DEFAULT;
    auto eh = Colors::DEFAULT;

    if (hovered && hovered->target == CBone::StartJoint)
      sh = Colors::HIGHLIGHT;
    else if (hovered && hovered->target == CBone::EndJoint)
      eh = Colors::HIGHLIGHT;
    else if (hovered && hovered->target == CBone::Shaft) {
      ch = Colors::HIGHLIGHT;
      sh = Colors::HIGHLIGHT;
      eh = Colors::HIGHLIGHT;
    }

    if (ECS::Mutate<CBone, glm::vec4>(
            registry, c, glm::mix(c, ch, ANIMATION_SPEED * deltaTime)) &&
        ECS::Mutate<CBone, glm::vec4>(
            registry, s, glm::mix(s, sh, ANIMATION_SPEED * deltaTime)) &&
        ECS::Mutate<CBone, glm::vec4>(
            registry, e, glm::mix(e, eh, ANIMATION_SPEED * deltaTime)))
      m_Registry->ClearChanged<CHovered>();
  }

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

    auto bones = m_Registry->GetEntities("bone");

    for (auto bone : bones) {
      auto cBone = bone->Get<CBone>();

      CBone::Part part = HoveredOver(cBone, mouse);

      auto cHovered = bone->Get<CHovered>();

      if (part != CBone::Part::None) {
        if (cHovered)
          cHovered->target = part;
        else
          bone->Add<CHovered>(part);
      } else if (cHovered)
        bone->Free<CHovered>();

      UpdateColor(m_Registry, cBone, cHovered, data->deltaTime);
    }
  }
};