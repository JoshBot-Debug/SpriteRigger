#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"

class TransformSystem
{
public:
  TransformSystem() = default;
  ~TransformSystem() = default;

  void update(float deltaTime, Registry *registry, Mouse *mouse);
};