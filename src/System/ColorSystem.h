#pragma once

#include "Viewport/viewport.h"
#include "ECS/registry.h"
#include "Input/mouse.h"

class ColorSystem
{ 
public:
  void update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse);
};