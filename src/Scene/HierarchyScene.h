#pragma once

#include "Scene/Scene.h"
#include "ECS/Registry.h"

#include "Controller/Controller.h"

class HierarchyScene : public Scene
{
private:
  Registry *registry;
  Controller *controller;
  
public:
  HierarchyScene() = default;
  ~HierarchyScene() = default;

  void onInitialize(Registry *registry, Controller *controller);
  void onDraw(float deltaTime) override;
};