#include "ViewportLayer.h"

#include "Utility.h"

ViewportLayer::ViewportLayer(State *state) : m_State(state) {}

void ViewportLayer::OnAttach() {}

void ViewportLayer::OnRender() {
  ImGui::Begin("Viewport", nullptr, ImGuiBackendFlags_None);

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

std::string ViewportLayer::GetName() { return "ViewportLayer"; }

void ViewportLayer::Save(Serializer &serializer) {}

void ViewportLayer::Restore(Serializer &serializer) {}
