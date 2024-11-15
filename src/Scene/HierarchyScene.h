#pragma once

#include "Scene/Scene.h"
#include "ECS/Registry.h"
#include "Controller/Controller.h"

class HierarchyScene : public Scene
{
private:
  Registry *registry;
  Controller controller;

public:
  HierarchyScene(Registry *registry) : registry(registry), controller(registry) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};