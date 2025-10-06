#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Application/Components.h"

class BoneInteractionSystem : public ECS::System {
private:
  float m_Speed = 6.0f;
  ECS::Registry *m_Registry;

public:
  double deltaTime;
  ImVec2 mouse;

  void OnAttach(ECS::Registry *registry) { m_Registry = registry; };

  void Update() override {
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
