#pragma once

#include <SDL3/SDL.h>
#include "MouseEventHandler/mouse_event_handler.h"

enum PressableEvent
{
  PRESSABLE_ON_PRESS,
  PRESSABLE_ON_PRESS_IN,
  PRESSABLE_ON_PRESS_OUT,
  PRESSABLE_ON_PRESS_LONG,
};

class Pressable
{
private:
  using Callback = std::function<void()>;
  MouseEventHandler &mouse_event_handler;
  std::map<PressableEvent, Callback> callbacks;

public:
  Pressable(MouseEventHandler &mouse_event_handler);
  virtual ~Pressable();

  virtual void onPress() {};
  virtual void onPressIn() {};
  virtual void onPressOut() {};
  virtual void onPressLong() {};

  void setBoundingRect(SDL_FRect *rect);
};