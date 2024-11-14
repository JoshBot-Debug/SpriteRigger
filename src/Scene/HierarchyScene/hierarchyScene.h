#pragma once

#include "imgui.h"
#include "app.h"
#include "Scene/scene.h"
#include "Component/components.h"
#include "Controller/armatureController.h"

class HierarchyScene : public Scene
{
private:
  App *app;
  ArmatureController armatureController;

public:
  HierarchyScene(App *app) : app(app), armatureController(this->app->getRegistry()) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};