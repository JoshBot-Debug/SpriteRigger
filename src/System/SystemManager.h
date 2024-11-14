#pragma once

#include "Viewport/viewport.h"
#include "ECS/registry.h"
#include "Input/mouse.h"
#include "System/RenderSystem.h"
#include "System/TransformSystem.h"
#include "System/ColorSystem.h"

class SystemManager
{
private:
  RenderSystem *render;
  TransformSystem *transform;
  ColorSystem *color;

public:
  SystemManager();
  ~SystemManager() = default;

  void update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse);

  void draw(float deltaTime, Registry *registry);
};