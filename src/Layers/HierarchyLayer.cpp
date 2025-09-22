#include "HierarchyLayer.h"

#include "Application/Rigger.h"

HierarchyLayer::HierarchyLayer(State *state, ServiceLocator *services)
    : m_State(state), m_Services(services) {}

void HierarchyLayer::OnAttach() {

  auto rigger = m_Services->Get<Rigger>();

  m_ContextMenu.Register({
      .id = "contextMenu",
      .items = {{
          .name = "New bone",
          .shortcut = "Ctrl B",
          .onClick = [&rigger]() { rigger->NewBone(); },
      }},
  });

  Window::RegisterShortcut({
      .ctrl = true,
      .key = ImGuiKey_B,
      .callback = [&rigger](Window *window) { rigger->NewBone(); },
  });
}

void HierarchyLayer::OnUpdate(float deltaTime) {}

void HierarchyLayer::OnRender() {
  ImGui::ShowDemoWindow();

  ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);

  m_ContextMenu.Render();

  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}
