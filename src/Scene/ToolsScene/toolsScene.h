#pragma once

#include "imgui.h"
#include "app.h"
#include "Scene/scene.h"
#include "Entity/components.h"

class ToolsScene : public Scene
{
private:
  App *app;

public:
  ToolsScene(App *app) : app(app) {}
  ~ToolsScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};