#pragma once

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"
#include "Render/Camera/OrthographicCamera.h"

class TransformSystem
{
public:
  TransformSystem() = default;
  ~TransformSystem() = default;

  void update(float deltaTime, Registry *registry, Mouse *mouse, OrthographicCamera *camera);
};