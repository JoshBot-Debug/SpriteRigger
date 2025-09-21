#include "HierarchyLayer.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {
  m_ContextMenu.Register({.id = "contextMenu",
                          .items = {
                              {
                                .name = "New bone",
                                .shortcut = "Ctrl N",
                                .onClick = []() {},
                              },
                          }});
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
