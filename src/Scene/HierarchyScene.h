#pragma once

#include "App.h"
#include "Scene/Scene.h"
#include "Controller/Controller.h"

class HierarchyScene : public Scene
{
private:
  App *app;
  Controller controller;

public:
  HierarchyScene(App *app) : app(app), controller(this->app->getRegistry()) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};