#pragma once

#include <glm/glm.hpp>
#include <tuple>

#include "imgui.h"

#include "ECS2/Registry.h"
#include "ECS2/System.h"
#include "ECS2/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class ColorInterpolationSystem : public ECS2::System {
private:
  Grid *m_Grid = nullptr;
  ECS2::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

private:
  /**
   * @brief Determines the highlight color for a specific bone part.
   *
   * Returns the appropriate highlight color for a given bone part based on the
   * currently targeted part. If the target matches the current part or the
   * target is the entire shaft, the highlight color is returned. Otherwise,
   * the base color is preserved.
   *
   * @param part   The specific bone part to evaluate (StartJoint, Shaft, or
   * EndJoint).
   * @param target The bone part currently being targeted for highlighting.
   * @param base   The base color to return when no highlight applies.
   * @return The resulting color for this bone part.
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
   * @brief Updates the color of a specific bone part based on highlight state.
   *
   * Smoothly transitions the given color toward the appropriate highlight.
   *
   * @param part   The bone part to modify (StartJoint, Shaft, or EndJoint).
   * @param color  Reference to the current color of this bone part.
   * @param target The bone part being highlighted.
   * @param skip   The bone part to exclude from modification.
   * @param lerp   The interpolation factor controlling animation speed.
   * @return True if the color was successfully updated; otherwise false.
   */
  bool Highlight(ECS2::Entity *entity, CBone::Part part, glm::vec4 &color,
                 CBone::Part target, bool skip, float lerp) {
    if (skip)
      return true;
    glm::vec4 next = GetHighlightColor(part, target, Colors::DEFAULT);
    return ECS2::Mutate<CBone, glm::vec4>(entity, color,
                                          Animation::Lerp(color, next, lerp));
  };

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

    float speed = ANIMATION_SPEED * data->deltaTime;

    for (auto [entity, cHovered] : m_Registry->GetChanged<EBone, CHovered>()) {
      if (auto cBone = entity->Get<CBone>()) {
        auto cSelected = entity->Get<CSelected>();

        if (cSelected && entity->MarkedForRemoval<CSelected>()) {
          entity->Remove<CSelected>();
          cSelected = nullptr;
        }

        auto &c = cBone->color;
        auto &s = cBone->joints[CBone::StartJoint].color;
        auto &e = cBone->joints[CBone::EndJoint].color;
        auto target = cHovered->target;
        auto skip = cSelected ? cSelected->target : CBone::None;

        bool mc = Highlight(entity, CBone::Shaft, c, target,
                            skip == CBone::Shaft, speed);
        bool ms =
            Highlight(entity, CBone::StartJoint, s, target,
                      skip == CBone::StartJoint || skip == CBone::Shaft, speed);
        bool me =
            Highlight(entity, CBone::EndJoint, e, target,
                      skip == CBone::EndJoint || skip == CBone::Shaft, speed);

        if (mc && ms && me)
          entity->ClearChanged<CHovered>();
      }
    }

    for (auto [entity, cSelected] :
         m_Registry->GetChanged<EBone, CSelected>()) {
      if (auto cBone = entity->Get<CBone>()) {

        auto &c = cBone->color;
        auto &s = cBone->joints[CBone::StartJoint].color;
        auto &e = cBone->joints[CBone::EndJoint].color;
        auto target = cSelected->target;

        bool mc = Highlight(
            entity, CBone::Shaft, c, target,
            target == CBone::StartJoint || target == CBone::EndJoint, speed);

        bool ms = Highlight(entity, CBone::StartJoint, s, target,
                            target == CBone::EndJoint, speed);

        bool me = Highlight(entity, CBone::EndJoint, e, target,
                            target == CBone::StartJoint, speed);

        if (mc && ms && me)
          entity->ClearChanged<CSelected>();
      }
    }
  }
};