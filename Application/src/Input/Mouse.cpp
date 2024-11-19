#include "Mouse.h"

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

void Mouse::onEvent(SDL_Event *event)
{
  switch (event->type)
  {
  case SDL_EVENT_MOUSE_MOTION:
    this->position.x = event->button.x;
    this->position.y = event->button.y;
    this->position -= *this->offset;
    this->setState(MouseState::MOVING);
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (event->button.button == SDL_BUTTON_LEFT)
      this->setState(MouseState::PRESS_LEFT);
    if (event->button.button == SDL_BUTTON_RIGHT)
      this->setState(MouseState::PRESS_RIGHT);
    if (event->button.button == SDL_BUTTON_MIDDLE)
      this->setState(MouseState::PRESS_MIDDLE);
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    this->setState(MouseState::RELEASED);
    break;
  default:
    break;
  }
}

void Mouse::setOffset(glm::vec2 *offset)
{
  this->offset = offset;
}
