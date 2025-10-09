#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Common.h"

class SelectSystem : public ECS::System {
private:
  Grid *m_Grid = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

public:
  ~SelectSystem() { m_Registry = nullptr; }

  void Initialize(ECS::Registry *registry, Grid *grid,
                  OrthographicCamera *camera) {
    m_Registry = registry;
    m_Grid = grid;
    m_Camera = camera;
  };

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    if (!m_Registry->Has<CHovered>()) {
      if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        m_Registry->Remove<CSelected>();
      return;
    }

    for (auto [id, cHovered] : m_Registry->Get<CHovered>())
      if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        m_Registry->Add<CSelected>(id, cHovered->target);
  }
};