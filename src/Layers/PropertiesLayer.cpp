#include "PropertiesLayer.h"

#include "Application/Components.h"
#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include <glm/gtc/type_ptr.hpp>

PropertiesLayer::PropertiesLayer(State* state) : m_State(state)
{
}

void PropertiesLayer::OnAttach()
{
  m_Registry = ServiceLocator::Get<ECS::Registry>();
}

void PropertiesLayer::OnUpdate(float deltaTime)
{
  auto selectedBones = m_Registry->Get<CBone, CSelected>();

  if (selectedBones.size() == 1)
    m_Entity = selectedBones[0].first;
  else
    m_Entity = nullptr;
}

void PropertiesLayer::OnRender()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2({0, 0}));
  ImGui::Begin("Properties ##SelectedBoneProperties");
  ImGui::PopStyleVar();

  if (m_Entity)
  {
    auto* hierarchy = m_Entity->Get<CHierarchy>();
    auto* bone = m_Entity->Get<CBone>();
    
    ImGui::Text("%s", hierarchy->name);
    ImGui::InputText("Name", hierarchy->name, IM_ARRAYSIZE(hierarchy->name), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::ColorPicker4("Color", glm::value_ptr(bone->color));
    ImGui::InputFloat("Thickness", &bone->thickness, 0.01f, 0.1f);
  }

  ImGui::End();
}

void PropertiesLayer::OnDetach()
{
  m_State = nullptr;
}

void PropertiesLayer::Save(Serializer& serializer)
{
}

void PropertiesLayer::Restore(Serializer& serializer)
{
}