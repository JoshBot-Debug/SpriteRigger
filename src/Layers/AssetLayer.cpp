#include "AssetLayer.h"

AssetLayer::AssetLayer(State *state) : m_State(state) {}

void AssetLayer::OnAttach() {}

void AssetLayer::OnRender() {
  ImGui::Begin("Assets", nullptr, ImGuiBackendFlags_None);
  ImGui::End();
}

void AssetLayer::OnDetach() { m_State = nullptr; }

std::string AssetLayer::GetName() { return "AssetLayer"; }

void AssetLayer::Save(Serializer &serializer) {}

void AssetLayer::Restore(Serializer &serializer) {}
