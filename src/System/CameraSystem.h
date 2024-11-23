#pragma once

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Render/Camera/OrthographicCamera.h"

class CameraSystem
{
private:
  float speed = 200.0f;
  glm::vec2 direction = {0.0f, 0.0f};

public:
  CameraSystem() = default;
  ~CameraSystem() = default;

  void update(float deltaTime, OrthographicCamera &camera, Mouse &mouse, Keyboard &keyboard);
};