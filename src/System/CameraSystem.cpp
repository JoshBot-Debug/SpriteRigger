#include "CameraSystem.h"

#include "Utility.h"
#include "Component/Component.h"

void CameraSystem::update(float deltaTime, OrthographicCamera &camera, Mouse &mouse, Keyboard &keyboard)
{
  direction.x = 0.0f;
  direction.y = 0.0f;

  if (keyboard.isPressed(Key::LEFT))
    direction.x = -1.0f;

  if (keyboard.isPressed(Key::RIGHT))
    direction.x = 1.0f;

  if (keyboard.isPressed(Key::UP))
    direction.y = -1.0f;

  if (keyboard.isPressed(Key::DOWN))
    direction.y = 1.0f;

  camera.move(speed * direction.x * deltaTime, speed * direction.y * deltaTime);
}
