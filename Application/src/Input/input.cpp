#include "input.h"

void Input::onEvent(SDL_Event *event)
{
  switch (event->type)
  {
  case SDL_EVENT_MOUSE_MOTION:
    this->mouse.position.x = event->button.x;
    this->mouse.position.y = event->button.y;
    this->mouse.setState(MouseState::MOVING);
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (event->button.button == SDL_BUTTON_LEFT)
      this->mouse.setState(MouseState::PRESS_LEFT);
    if (event->button.button == SDL_BUTTON_RIGHT)
      this->mouse.setState(MouseState::PRESS_RIGHT);
    if (event->button.button == SDL_BUTTON_MIDDLE)
      this->mouse.setState(MouseState::PRESS_MIDDLE);
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    this->mouse.setState(MouseState::RELEASED);
    break;
  default:
    break;
  }
}

Mouse *Input::getMouse()
{
  return &this->mouse;
}
