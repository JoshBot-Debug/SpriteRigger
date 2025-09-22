#include "AssetLayer.h"

AssetLayer::AssetLayer(State *state, ServiceLocator *services) : m_State(state), m_Services(services) {}

void AssetLayer::OnAttach() {}

void AssetLayer::OnRender() {
  ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_None);
  ImGui::End();
}

void AssetLayer::OnDetach() { m_State = nullptr; }

void AssetLayer::Save(Serializer &serializer) {}

void AssetLayer::Restore(Serializer &serializer) {}
