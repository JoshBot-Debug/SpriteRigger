#include "mouse.h"
#include <string>

void Mouse::press(EntityID entity, glm::vec2 position)
{
  this->entity = entity;
  this->entityPosition = position;
  this->dragStart = this->position;
  this->isFocused = true;
  this->isDragging = true;
}

void Mouse::unfocus(EntityID entity)
{
  if (entity == -1)
    this->isFocused = false;
  if (entity == this->entity)
    this->isFocused = false;
}

void Mouse::setState(MouseState state)
{
  this->state = state;

  switch (state)
  {
  case MouseState::MOVING:
    break;
  default:
    this->isDragging = false;
    break;
  }
}

MouseEntityState Mouse::getMouseEntityState(EntityID entity)
{
  MouseEntityState state;
  state.position = this->entityPosition + this->position - this->dragStart;
  state.isDragging = this->entity == entity && this->isDragging;
  state.isFocused = this->entity == entity && this->isFocused;
  return state;
}