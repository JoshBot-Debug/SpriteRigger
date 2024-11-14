#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"

class TransformSystem
{ 
public:
  void update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse);
};