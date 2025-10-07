#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Window/Window.h"
#include "Camera/Components/Grid.h"

class BoneInteractionSystem : public ECS::System {
private:
  float m_Speed = 6.0f;
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;

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
      if (b->Intersects(mouse.x, mouse.y))
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->color,
            glm::mix(b->color, glm::vec4(1, 1, 0, 1), m_Speed * deltaTime));
      else
        ECS::Mutate<CBone, glm::vec4>(
            m_Registry, b->color,
            glm::mix(b->color, glm::vec4(1.0f), m_Speed * deltaTime));
    }
  }
};
