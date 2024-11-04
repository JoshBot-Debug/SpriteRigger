#pragma once

#include "imgui.h"
#include "application.h"
#include "Scene/scene.h"
#include "Entity/components.h"

class ToolsScene : public Scene
{
public:
  ToolsScene(Application *application) : Scene(application) {}
  ~ToolsScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};