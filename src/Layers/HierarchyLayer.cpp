#include "HierarchyLayer.h"

#include "Application/Rigger.h"
#include "ServiceLocator/ServiceLocator.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {

  auto rigger = ServiceLocator::Get<Rigger>();

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
