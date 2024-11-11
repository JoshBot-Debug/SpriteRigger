#include "mouse.h"

const std::tuple<EntityID, Vec2> DRAG_NONE = {-1, Vec2{-1, -1}};

void Mouse::resetDrag()
{
  this->dragEntity = -1;
  this->dragEntityZIndex = -1;
  this->dragEntityPosition = Vec2{-1, -1};
  this->dragStart = Vec2{-1, -1};
}

void Mouse::press(EntityID entity, Vec2 position, int zIndex)
{
  if (this->dragEntity != -1)
    if (this->dragEntityZIndex < zIndex)
      return;

  this->dragEntityZIndex = zIndex;
  this->dragEntityZIndex = entity;
  this->dragEntityPosition = position;
  this->dragStart = this->position;
}

void Mouse::setState(MouseState state)
{
  this->state = state;

  switch (state)
  {
  case MouseState::PRESS_RIGHT:
    this->resetDrag();
    break;
  case MouseState::PRESS_MIDDLE:
    this->resetDrag();
    break;
  case MouseState::RELEASED:
    this->resetDrag();
    break;
  default:
    break;
  }
}

std::tuple<EntityID, Vec2> Mouse::drag()
{
  if (this->state != MouseState::MOVING)
    return DRAG_NONE;

  return std::tuple<EntityID, Vec2>({this->dragEntity, this->dragEntityPosition + this->position - this->dragStart});
}