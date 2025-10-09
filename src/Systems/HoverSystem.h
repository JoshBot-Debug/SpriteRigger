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

  // May want to create a ColorInterpolationSystem to interpolate colors
  void UpdateColor(ECS::Registry *registry, CBone *bone, CHovered *hovered,
                   float deltaTime) {
    if (!m_Registry->AnyChanged<CHovered>())
      return;

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

    float speed = ANIMATION_SPEED * deltaTime;

    bool mc = ECS::Mutate<CBone, glm::vec4>(registry, c,
                                            Animation::Lerp(c, ch, speed));

    bool ms = ECS::Mutate<CBone, glm::vec4>(registry, s,
                                            Animation::Lerp(s, sh, speed));
    bool me = ECS::Mutate<CBone, glm::vec4>(registry, e,
                                            Animation::Lerp(e, eh, speed));

    if (mc && ms && me)
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
      auto cHovered = bone->Get<CHovered>();

      CBone::Part part = HoveredOver(cBone, mouse);

      if (!cHovered)
        cHovered = bone->Add<CHovered>(part);
      else if (part == CBone::Part::None)
        cHovered = bone->Remove<CHovered>();
      else
        ECS::Mutate<CHovered, CBone::Part>(m_Registry, cHovered->target, part);

      UpdateColor(m_Registry, cBone, cHovered, data->deltaTime);
    }
  }
};