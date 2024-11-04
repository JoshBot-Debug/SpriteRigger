#include "toolsScene.h"
#include <imgui_internal.h>

void ToolsScene::onInitialize()
{
  Registry *registry = this->application->getRegistry();

  Entity *bone1 = registry->createEntity("Bone");
  bone1->add<PropertiesComponent>("Bone 1", 1);
  bone1->add<MeshComponent>(Vec2{100, 100});
  bone1->add<TransformComponent>(Vec2{100, 100});
  bone1->add<ColorComponent>(Vec4{255, 255, 255, 255});

  Entity *bone2 = registry->createEntity("Bone");
  bone2->add<PropertiesComponent>("Bone 2", 2);
  bone2->add<MeshComponent>(Vec2{100, 100});
  bone2->add<TransformComponent>(Vec2{100, 300});
  bone2->add<ColorComponent>(Vec4{255, 0, 0, 255});
}

void ToolsScene::onInput(SDL_Event *event, float deltaTime)
{
  if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_RIGHT)
  {
  }
}

void ToolsScene::onUpdate(float deltaTime)
{
}

void ToolsScene::onDraw(float deltaTime)
{
  Registry *registry = this->application->getRegistry();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Hierarchy", nullptr);
  
  ImGui::End();

  ImGui::ShowDemoWindow();
}

void ToolsScene::onCleanUp()
{
}
