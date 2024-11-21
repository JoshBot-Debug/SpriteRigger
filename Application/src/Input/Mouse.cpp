#include "Mouse.h"

void Mouse::updateMousePosition(float x, float y)
{
  this->position.x = x;
  this->position.y = y;
  
  if (!(this->bounds.position && this->bounds.dimensions))
    return;

  this->bounds.state = this->intersects(*this->bounds.position, *this->bounds.dimensions)
                           ? MouseBounds::IN_BOUNDS
                           : MouseBounds::OUT_OF_BOUNDS;

  this->position -= *this->bounds.position;

  if (this->bounds.origin == MouseOrigin::CENTER)
    this->position -= *this->bounds.dimensions / 2.0f;
}

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
    this->updateMousePosition(event->button.x, event->button.y);
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

void Mouse::setBounds(glm::vec2 *position, glm::vec2 *dimensions, MouseOrigin origin)
{
  this->bounds = {position, dimensions, origin};
}

bool Mouse::intersects(const glm::vec2 &position, const glm::vec2 &size)
{
  return (this->position.x >= position.x && this->position.x <= position.x + size.x) &&
         (this->position.y >= position.y && this->position.y <= position.y + size.y);
}

MouseBounds Mouse::getMouseBoundsState()
{
  return this->bounds.state;
}
