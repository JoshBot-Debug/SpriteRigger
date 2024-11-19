#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"

class ColorSystem
{
public:
  ColorSystem() = default;
  ~ColorSystem() = default;

  void update(float deltaTime, Registry *registry, Mouse *mouse);
};