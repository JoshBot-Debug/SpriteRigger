#include "ViewportLayer.h"

#include "Utility.h"

ViewportLayer::ViewportLayer(State *state, ServiceLocator *services) : m_State(state), m_Services(services) {}

void ViewportLayer::OnAttach() {}

void ViewportLayer::OnUpdate(float deltaTime) {}

void ViewportLayer::OnRender() {
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {}

void ViewportLayer::Restore(Serializer &serializer) {}
