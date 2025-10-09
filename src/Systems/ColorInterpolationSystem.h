#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class ColorInterpolationSystem : public ECS::System {
private:
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

public:
  ~ColorInterpolationSystem() { m_Registry = nullptr; }

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

    if (!m_Registry->AnyChanged<CHovered>())
      return;

    for (auto bone : bones) {
      auto cBone = bone->Get<CBone>();
      auto cHovered = bone->Get<CHovered>();

      auto &c = cBone->color;
      auto &s = cBone->joints[CBone::StartJoint].color;
      auto &e = cBone->joints[CBone::EndJoint].color;

      auto ch = Colors::DEFAULT;
      auto sh = Colors::DEFAULT;
      auto eh = Colors::DEFAULT;

      if (cHovered) {
        if (cHovered->target == CBone::StartJoint)
          sh = Colors::HIGHLIGHT;
        else if (cHovered->target == CBone::EndJoint)
          eh = Colors::HIGHLIGHT;
        else if (cHovered->target == CBone::Shaft) {
          ch = Colors::HIGHLIGHT;
          sh = Colors::HIGHLIGHT;
          eh = Colors::HIGHLIGHT;
        }
      }

      float speed = ANIMATION_SPEED * data->deltaTime;

      bool mc = ECS::Mutate<CBone, glm::vec4>(m_Registry, c,
                                              Animation::Lerp(c, ch, speed));

      bool ms = ECS::Mutate<CBone, glm::vec4>(m_Registry, s,
                                              Animation::Lerp(s, sh, speed));
      bool me = ECS::Mutate<CBone, glm::vec4>(m_Registry, e,
                                              Animation::Lerp(e, eh, speed));

      if (mc && ms && me)
        m_Registry->ClearChanged<CHovered>();
    }
  }
};