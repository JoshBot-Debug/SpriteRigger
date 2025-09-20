#include "HierarchyLayer.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {}

void HierarchyLayer::OnRender() {
  ImGui::Begin("Hierarchy", nullptr, ImGuiBackendFlags_None);
  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

std::string HierarchyLayer::GetName() { return "HierarchyLayer"; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}
