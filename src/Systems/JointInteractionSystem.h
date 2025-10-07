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
      if (b->IntersectsJoint(b->start, mouse.x, mouse.y))
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->sColor,
            glm::mix(b->sColor, glm::vec4(1, 1, 0, 1), m_Speed * deltaTime));
      else
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->sColor,
            glm::mix(b->sColor, glm::vec4(1.0f), m_Speed * deltaTime));

      if (b->IntersectsJoint(b->end, mouse.x, mouse.y))
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->eColor,
            glm::mix(b->eColor, glm::vec4(1, 1, 0, 1), m_Speed * deltaTime));
      else
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->eColor,
            glm::mix(b->eColor, glm::vec4(1.0f), m_Speed * deltaTime));
    }
  }
};
