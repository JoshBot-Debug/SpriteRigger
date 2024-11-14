#include "HierarchyScene.h"
#include <imgui_internal.h>

void HierarchyScene::onInitialize() {}

void HierarchyScene::onInput(SDL_Event *event, float deltaTime) {}

void HierarchyScene::onUpdate(float deltaTime) {}

void HierarchyScene::onDraw(float deltaTime)
{
  Registry *registry = this->app->getRegistry();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Tools", nullptr);

  if (ImGui::BeginPopupContextWindow("Tools Context"))
  {
    if (ImGui::MenuItem("Create armature"))
    {
      this->controller.createArmature();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  for (auto armature : registry->get<CArmature>())
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

void HierarchyScene::onCleanUp() {}
