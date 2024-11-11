#pragma once

#include "imgui.h"
#include "app.h"
#include "Scene/scene.h"
#include "Entity/components.h"
#include "Manager/armatureManager.h"

class HierarchyScene : public Scene
{
private:
  App *app;
  ArmatureManager armatureManager;

public:
  HierarchyScene(App *app) : app(app), armatureManager(this->app->getRegistry()) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};