#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Window/Window.h"

class BoneInteractionSystem : public ECS::System {
private:
  float m_Speed = 6.0f;
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;

  glm::vec4 GetHighlightColor(bool intersects) {
    return intersects ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(1.0f);
  }

public:
  ~BoneInteractionSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid) {
    m_Registry = registry;
    m_Grid = grid;
  };

  void Update() override {
    float deltaTime = static_cast<float>(Window::GetDeltaTime());
    ImVec2 mouse = m_Grid->GetMouseCoords();

    auto bones = m_Registry->Get<CBone>();

    for (auto b : bones) {
      glm::vec4 color = GetHighlightColor(b->Intersects(mouse.x, mouse.y));
      ECS::Mutate<CBone, glm::vec4>(
          m_Registry, b->color, glm::mix(b->color, color, m_Speed * deltaTime));
    }
  }
};
