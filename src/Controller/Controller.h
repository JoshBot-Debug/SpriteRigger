#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"

#include "System/ColorSystem.h"
#include "System/RenderSystem.h"
#include "System/TransformSystem.h"

#include "Component/Component.h"

class Controller
{
private:
  Registry *registry;

  ColorSystem *colorSystem;
  RenderSystem *renderSystem;
  TransformSystem *transformSystem;

public:
  Controller();
  ~Controller();

  void onInitialize(Registry *registry, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem);

  void createArmature();
};