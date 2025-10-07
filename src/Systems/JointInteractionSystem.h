#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"
#include "Window/Window.h"

class JointInteractionSystem : public ECS::System {
private:
  float m_Speed = 6.0f;
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

  glm::vec4 GetHighlightColor(bool intersects) {
    return intersects ? glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(1.0f);
  }

public:
  ImVec2 viewportSize;

  ~JointInteractionSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Registry = registry;
    m_Grid = grid;
    m_Camera = camera;
  };

  void Update() override {
    ImGuiIO &io = ImGui::GetIO();
    float deltaTime = static_cast<float>(Window::GetDeltaTime());
    ImVec2 mouse = m_Grid->GetMouseCoords();
    float deltaMouseX =
        io.MouseDelta.x * m_Camera->Zoom * 2.0f / viewportSize.x;
    float deltaMouseY =
        -io.MouseDelta.y * m_Camera->Zoom * 2.0f / viewportSize.y;

    auto bones = m_Registry->Get<CBone>();

    for (auto b : bones) {
      bool intersectsStart = b->IntersectsJoint(b->start, mouse.x, mouse.y);
      bool intersectsEnd = b->IntersectsJoint(b->end, mouse.x, mouse.y);

      if (intersectsStart && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ECS::Mutate<CBone, glm::vec2>(m_Registry, b->start,
                                      b->start +
                                          glm::vec2(deltaMouseX, deltaMouseY));
      }

      if (intersectsEnd && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ECS::Mutate<CBone, glm::vec2>(
            m_Registry, b->end, b->end + glm::vec2(deltaMouseX, deltaMouseY));
      }

      glm::vec4 startColor = GetHighlightColor(intersectsStart);
      glm::vec4 endColor = GetHighlightColor(intersectsEnd);

      ECS::Mutate<CBone, glm::vec4>(
          m_Registry, b->sColor,
          glm::mix(b->sColor, startColor, m_Speed * deltaTime));

      ECS::Mutate<CBone, glm::vec4>(
          m_Registry, b->eColor,
          glm::mix(b->eColor, endColor, m_Speed * deltaTime));
    }
  }
};
