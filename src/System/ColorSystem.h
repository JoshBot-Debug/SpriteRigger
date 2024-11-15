#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"

class ColorSystem
{
private:
  Registry *registry;
  Mouse *mouse;

public:
  ColorSystem(Registry *registry, Mouse *mouse) : registry(registry), mouse(mouse) {};

  void update(float deltaTime);
};