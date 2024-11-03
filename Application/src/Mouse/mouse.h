#pragma once

#include <common.h>

enum class MouseState
{
  RELEASED,
  PRESSED_WHEEL,
  PRESSED_LEFT,
  PRESSED_RIGHT,
};

enum class MouseButton
{
  LEFT,
  RIGHT,
  WHEEL,
};

class Mouse
{
private:
  Vec2 position;
  MouseState state;

public:
  Mouse() {}
  ~Mouse() {}

  void setPosition(float x, float y)
  {
    this->position.x = x;
    this->position.y = y;
  }

  void setState(MouseState state)
  {
    this->state = state;
  }

  bool isPressed(MouseButton button)
  {
    if (button == MouseButton::LEFT)
      return this->state == MouseState::PRESSED_LEFT;
    if (button == MouseButton::RIGHT)
      return this->state == MouseState::PRESSED_RIGHT;
    if (button == MouseButton::WHEEL)
      return this->state == MouseState::PRESSED_WHEEL;
    return false;
  }

  Vec2 getPosition()
  {
    return this->position;
  }

  operator Vec2() const
  {
    return this->position;
  }
};