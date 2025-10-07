#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Window/Window.h"

class JointInteractionSystem : public ECS::System {
private:
  float m_Speed = 6.0f;
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;

  glm::vec4 GetHighlightColor(bool intersects) {
    return intersects ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(1.0f);
  }

public:
  ~JointInteractionSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid) {
    m_Registry = registry;
    m_Grid = grid;
  };

  void Update() override {
    float deltaTime = static_cast<float>(Window::GetDeltaTime());
    ImVec2 mouse = m_Grid->GetMouseCoords();

    auto bones = m_Registry->Get<CBone>();

    for (auto b : bones) {
      glm::vec4 startColor =
          GetHighlightColor(b->IntersectsJoint(b->start, mouse.x, mouse.y));
      glm::vec4 endColor =
          GetHighlightColor(b->IntersectsJoint(b->end, mouse.x, mouse.y));

      ECS::Mutate<CBone, glm::vec4>(
          m_Registry, b->sColor,
          glm::mix(b->sColor, startColor, m_Speed * deltaTime));

      ECS::Mutate<CBone, glm::vec4>(
          m_Registry, b->eColor,
          glm::mix(b->eColor, endColor, m_Speed * deltaTime));
    }
  }
};
