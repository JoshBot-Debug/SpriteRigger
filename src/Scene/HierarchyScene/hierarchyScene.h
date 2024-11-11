#pragma once

#include "imgui.h"
#include "app.h"
#include "Scene/scene.h"
#include "Entity/components.h"
#include "System/armatureSystem.h"

class HierarchyScene : public Scene
{
private:
  App *app;
  ArmatureSystem armatureSystem;

public:
  HierarchyScene(App *app) : app(app), armatureSystem(this->app->getRegistry()) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};