#pragma once

#include "Scene/Scene.h"
#include "ECS/Registry.h"

class HierarchyScene : public Scene
{
private:
  Registry *registry;

public:
  HierarchyScene(Registry *registry) : registry(registry) {}
  ~HierarchyScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};