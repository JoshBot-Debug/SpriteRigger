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

  CBone::Part HoveredPart(CBone *bone, glm::vec2 mouse) {
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
    float lerp = ANIMATION_SPEED * data->deltaTime;

    auto bones = m_Registry->GetEntities("bone");

    for (auto bone : bones) {

      auto cBone = bone->Get<CBone>();

      CBone::Part part = HoveredPart(cBone, mouse);

      auto cHovered = bone->Get<CHovered>();

      if (part != CBone::Part::None) {
        if (cHovered)
          cHovered->target = part;
        else
          bone->Add<CHovered>();
      } else
        bone->Free<CHovered>();

      // auto &start = bone->joints[CBone::StartJoint];
      // auto &end = bone->joints[CBone::EndJoint];

      // bool sji = Intersect::Circle(mouse, start.position, bone->thickness);
      // bool eji = Intersect::Circle(mouse, end.position, bone->thickness);
      // bool li =
      //     !sji && !eji &&
      //     Intersect::Line(mouse, start.position, end.position,
      //     bone->thickness);

    }
  }
};