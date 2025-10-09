#pragma once

#include <glm/glm.hpp>
#include <tuple>

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

private:
  /**
   * @param part What part of the bone is this, StartJoint, Shaft or EndJoint
   * @param target The part of the bone we want get the color for
   * @param base The base color if nothing matches
   */
  glm::vec4 GetHighlightColor(CBone::Part part, CBone::Part target,
                              const glm::vec4 &base) {
    if (target == CBone::Shaft)
      return Colors::HIGHLIGHT;
    if (target == part)
      return Colors::HIGHLIGHT;
    return base;
  };

  /**
   * @param part What part of the bone is this, StartJoint, Shaft or EndJoint
   * @param color A reference to the color variable for this joint
   * @param target The part of the bone we want to color
   * @param skip The part of the bone we want to skip, regardless of the target
   * @param lerp The animation time
   */
  bool Highlight(CBone::Part part, glm::vec4 &color, CBone::Part target,
                 CBone::Part skip, float lerp) {
    if (part == skip)
      return true;
    glm::vec4 next = GetHighlightColor(part, target, Colors::DEFAULT);
    return ECS::Mutate<CBone, glm::vec4>(m_Registry, color,
                                         Animation::Lerp(color, next, lerp));
  };

public:
  ~ColorInterpolationSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Registry = registry;
    m_Grid = grid;
    m_Camera = camera;
  };

  void Update(void *d) override {
    if (!m_Registry->AnyChanged<CHovered>())
      return;

    auto data = reinterpret_cast<SystemData *>(d);

    float speed = ANIMATION_SPEED * data->deltaTime;

    for (auto [eid, cHovered] : m_Registry->Get<CHovered>()) {
      if (auto cBone = m_Registry->Get<CBone>(eid)) {
        auto cSelected = m_Registry->Get<CSelected>(eid);

        auto &c = cBone->color;
        auto &s = cBone->joints[CBone::StartJoint].color;
        auto &e = cBone->joints[CBone::EndJoint].color;
        auto target = cHovered->target;
        auto skip = cSelected ? cSelected->target : CBone::None;

        bool mc = Highlight(CBone::Shaft, c, target, skip, speed);
        bool ms = Highlight(CBone::StartJoint, s, target, skip, speed);
        bool me = Highlight(CBone::EndJoint, e, target, skip, speed);

        if (mc && ms && me)
          m_Registry->ClearChanged<CHovered>();
      }
    }

    for (auto [eid, cSelected] : m_Registry->Get<CSelected>()) {
      if (auto cBone = m_Registry->Get<CBone>(eid)) {
        auto &c = cBone->color;
        auto &s = cBone->joints[CBone::StartJoint].color;
        auto &e = cBone->joints[CBone::EndJoint].color;
        auto target = cSelected->target;

        if (target == CBone::StartJoint)
          Highlight(CBone::StartJoint, s, target, CBone::None, speed);
        else if (target == CBone::EndJoint)
          Highlight(CBone::EndJoint, e, target, CBone::None, speed);
        else if (target == CBone::Shaft) {
          Highlight(CBone::Shaft, c, target, CBone::None, speed);
          Highlight(CBone::StartJoint, s, target, CBone::None, speed);
          Highlight(CBone::EndJoint, e, target, CBone::None, speed);
        }
      }
    }
  }
};