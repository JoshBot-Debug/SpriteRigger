#include "pressable.h"

Pressable::Pressable(MouseEventHandler &mouse_event_handler) : mouse_event_handler(mouse_event_handler)
{
  this->callbacks[PRESSABLE_ON_PRESS] = [this]()
  { this->onPress(); };

  this->callbacks[PRESSABLE_ON_PRESS_IN] = [this]()
  { this->onPressIn(); };

  this->callbacks[PRESSABLE_ON_PRESS_OUT] = [this]()
  { this->onPressOut(); };

  this->callbacks[PRESSABLE_ON_PRESS_LONG] = [this]()
  { this->onPressLong(); };

  this->mouse_event_handler.addEventListener(MouseEvent::LEFT_CLICK, this->callbacks[PRESSABLE_ON_PRESS]);
  this->mouse_event_handler.addEventListener(MouseEvent::LEFT_CLICK_IN, this->callbacks[PRESSABLE_ON_PRESS_IN]);
  this->mouse_event_handler.addEventListener(MouseEvent::LEFT_CLICK_OUT, this->callbacks[PRESSABLE_ON_PRESS_OUT]);
  this->mouse_event_handler.addEventListener(MouseEvent::LEFT_LONG_PRESS, this->callbacks[PRESSABLE_ON_PRESS_LONG]);
}

Pressable::~Pressable()
{
  this->mouse_event_handler.removeEventListener(MouseEvent::LEFT_CLICK, this->callbacks[PRESSABLE_ON_PRESS]);
  this->mouse_event_handler.removeEventListener(MouseEvent::LEFT_CLICK_IN, this->callbacks[PRESSABLE_ON_PRESS_IN]);
  this->mouse_event_handler.removeEventListener(MouseEvent::LEFT_CLICK_OUT, this->callbacks[PRESSABLE_ON_PRESS_OUT]);
  this->mouse_event_handler.removeEventListener(MouseEvent::LEFT_LONG_PRESS, this->callbacks[PRESSABLE_ON_PRESS_LONG]);
}

void Pressable::setBoundingRect(SDL_FRect *rect)
{
  this->mouse_event_handler.setBoundingRect(rect);
}