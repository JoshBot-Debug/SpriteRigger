#include "hierarchyScene.h"
#include <imgui_internal.h>

void HierarchyScene::onInitialize()
{
  Registry *registry = this->app->getRegistry();

  Entity *bone1 = registry->createEntity("Bone");
  bone1->add<PropertiesComponent>("Bone 1", 1);
  bone1->add<MeshComponent>(Vec2{100, 100});
  bone1->add<TransformComponent>(Vec2{100, 100});
  bone1->add<ColorComponent>(Vec4{255, 255, 255, 255});
}

void HierarchyScene::onInput(SDL_Event *event, float deltaTime)
{
  if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_RIGHT)
  {
  }
}

void HierarchyScene::onUpdate(float deltaTime)
{
}

void HierarchyScene::onDraw(float deltaTime)
{
  Registry *registry = this->app->getRegistry();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Tools", nullptr);

  if (ImGui::BeginPopupContextWindow("Tools Context"))
  {
    if (ImGui::MenuItem("Add bone")) {

      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();
}

void HierarchyScene::onCleanUp()
{
}
