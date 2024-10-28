#include "mouse_event_handler.h"

// 500ms, same as react native
const int LONG_PRESS_TIME = 500;

MouseEventHandler::MouseEventHandler() : state(MouseState::MOUSE_STATE_NONE), pressed_time(0), listeners({}), boundingRect(new SDL_FRect({0.0f, 0.0f, 0.0f, 0.0f})), point(new SDL_FPoint({0.0f, 0.0f})) {}

MouseEventHandler::~MouseEventHandler()
{
  delete this->point;
  this->boundingRect = nullptr;
  this->point = nullptr;
}

MouseState MouseEventHandler::getState()
{
  return this->state;
}

void MouseEventHandler::onEvent(SDL_Event event)
{
  if (event.type == SDL_EVENT_MOUSE_MOTION)
  {
    SDL_GetMouseState(&this->point->x, &this->point->y);
  }

  if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP)
  {
    if (!SDL_PointInRectFloat(this->point, this->boundingRect))
      return;

    bool left_button = event.button.button == SDL_BUTTON_LEFT;

    MouseEvent CLICK = left_button ? MouseEvent::LEFT_CLICK : MouseEvent::RIGHT_CLICK;
    MouseEvent CLICK_IN = left_button ? MouseEvent::LEFT_CLICK_IN : MouseEvent::RIGHT_CLICK_IN;
    MouseEvent CLICK_OUT = left_button ? MouseEvent::LEFT_CLICK_OUT : MouseEvent::RIGHT_CLICK_OUT;
    MouseState MOUSE_STATE_CLICK = left_button ? MouseState::MOUSE_STATE_LEFT_CLICK : MouseState::MOUSE_STATE_RIGHT_CLICK;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
      this->callListeners(CLICK_IN);
      this->state = MOUSE_STATE_CLICK;
      this->pressed_time = SDL_GetTicks() + LONG_PRESS_TIME;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
      this->callListeners(CLICK_OUT);

      if (this->state != MouseState::MOUSE_STATE_NONE)
      {
        this->callListeners(CLICK);
        this->state = MouseState::MOUSE_STATE_NONE;
      }
    }
  }
}

void MouseEventHandler::onEventLoopStart()
{
  if (this->pressed_time == 0)
    return;

  if (SDL_GetTicks() < this->pressed_time)
    return;

  this->pressed_time = 0;

  switch (this->state)
  {
  case MouseState::MOUSE_STATE_LEFT_CLICK:
    this->callListeners(MouseEvent::LEFT_LONG_PRESS);
    break;
  case MouseState::MOUSE_STATE_RIGHT_CLICK:
    this->callListeners(MouseEvent::RIGHT_LONG_PRESS);
    break;
  default:
    break;
  }
}

void MouseEventHandler::addEventListener(MouseEvent event, Callback callback)
{
  this->listeners[event].push_back(callback);
}

void MouseEventHandler::removeEventListener(MouseEvent event, Callback callback)
{
  auto &callbacks = this->listeners[event];
  callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
                                 [&callback](Callback storedCallback)
                                 {
                                   return storedCallback.target<void()>() == callback.target<void>();
                                 }),
                  callbacks.end());
}

void MouseEventHandler::callListeners(MouseEvent event)
{
  if (!this->hasListeners(event))
    return;

  for (const auto &callback : this->listeners[event])
    callback();
}

bool MouseEventHandler::hasListeners(MouseEvent event)
{
  auto it = listeners.find(event);
  return (it != listeners.end() && !it->second.empty());
}

void MouseEventHandler::setBoundingRect(SDL_FRect *rect)
{
  this->boundingRect = rect;
}

bool isPointInRect(SDL_FRect &rect, float x, float y)
{
  return (x >= rect.x && x <= rect.x + rect.w &&
          y >= rect.y && y <= rect.y + rect.h);
}