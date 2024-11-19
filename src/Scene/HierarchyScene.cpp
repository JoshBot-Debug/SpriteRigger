#include "HierarchyScene.h"

#include "imgui_internal.h"
#include "Component/Component.h"

void HierarchyScene::onInitialize(Registry *registry, Controller *controller)
{
  this->registry = registry;
  this->controller = controller;
}

void HierarchyScene::onDraw(float deltaTime)
{
  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Tools", nullptr);

  if (ImGui::BeginPopupContextWindow("Tools Context"))
  {
    if (ImGui::MenuItem("Create armature"))
    {
      this->controller->createArmature();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  for (auto armature : this->registry->get<CArmature>())
  {
    if (ImGui::TreeNode(armature->name.c_str()))
    {
      for (auto entityID : armature->bones)
      {
        CBone *bone = registry->get<CBone>(entityID);
        ImGui::Text("%s", bone->name.c_str());
      }
      ImGui::TreePop();
    }
  }

  ImGui::End();
}