#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"

class TransformSystem
{ 
private:
  Registry *registry;
  Mouse *mouse;

public:
  TransformSystem(Registry *registry, Mouse *mouse) : registry(registry), mouse(mouse) {};
  void update(float deltaTime);
};